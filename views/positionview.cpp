#include "positionview.h"
#include <QHeaderView>

PositionView::PositionView(QWidget *parent) :
    QWidget(parent)
{
    //Add or set Widgets
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QLabel *title = new QLabel(tr("Position"));

    m_positionTable = new QTableWidget(0, 4);
    //Position (name), Times (number of position), Points (calculated value), Trash icon
    m_positionTable->setHorizontalHeaderLabels(QStringList(tr("Position")) << tr("Times") << tr("Points") << "");
    m_positionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_positionTable->verticalHeader()->setVisible(false);
    m_positionTable->setShowGrid(false);

    //set an icon instead of "+"
    QWidget *addPositionWidget = new QWidget;
    QHBoxLayout *addPositionLayout = new QHBoxLayout;
    QPushButton *addPositionButton = new QPushButton("+");
    m_addPositionComboBox = new QComboBox;
    m_addPositionSpinBox = new QSpinBox;
    m_timesPointLabel = new QLabel(tr("x %1 points").arg(0));
    m_pointLabel = new QLabel(tr("Points: %1").arg(0));
    m_dateLabel = new QLabel(tr("For: %1").arg(QDate::currentDate().toString()));
    m_validateButton = new QPushButton(tr("Validate daily positions"));
    m_calendar = new QCalendarWidget;

    m_addPositionComboBox->setEditable(true);
    m_addPositionComboBox->setInsertPolicy(QComboBox::NoInsert);

    //Place Widgets
    //add addPosition widgets to layout
    addPositionWidget->setLayout(addPositionLayout);
    addPositionLayout->addWidget(m_addPositionComboBox);
    addPositionLayout->addWidget(m_addPositionSpinBox);
    addPositionLayout->addWidget(m_timesPointLabel);
    addPositionLayout->addWidget(addPositionButton);

    setLayout(mainLayout);
    mainLayout->addWidget(title);
    mainLayout->addWidget(m_positionTable);
    mainLayout->addWidget(addPositionWidget);

    mainLayout->addWidget(m_pointLabel);
    mainLayout->addWidget(m_dateLabel);
    mainLayout->addWidget(m_validateButton);
    mainLayout->addWidget(m_calendar);

    //Signal and slot connections
    //a signal mapper will be usefull for delete button of each position/serie
    m_signalMapper = new QSignalMapper(this);

    connect(addPositionButton, SIGNAL(clicked()), this, SLOT(addPosition()));
    connect(m_addPositionComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(positionChanged(QString)));
    connect(m_positionTable, SIGNAL(cellClicked(int,int)), this, SLOT(positionTableCellClicked(int,int)));
    connect(m_positionTable, SIGNAL(cellChanged(int,int)), this, SLOT(updatePoints(int, int)));
    connect(m_validateButton, SIGNAL(clicked()), this, SLOT(validateDailyPositions()));
    connect(m_calendar, SIGNAL(clicked(QDate)), this, SLOT(dateSelected(QDate)));

    //Populate position list
    updatePositions();
    if (m_positions.size() > 0) {
        positionChanged(QString::fromStdString(m_positions.front()->name()));
    }
}

PositionView::~PositionView()
{
}

void PositionView::updatePositions()
{
    //clean-up
    m_positionList.clear();
    m_positions.clear();
    m_addPositionComboBox->clear();

    //find positions from database
    QSqlQuery selectPosition("SELECT id, name, point FROM yoga_point WHERE is_serie IS NOT 1");
    //keep how many position are available
    int positionCount = 0;
    while (selectPosition.next()) {
        int positionId = selectPosition.value(0).toInt();
        QString positionName = selectPosition.value(1).toString();
        int positionPoint = selectPosition.value(2).toInt();
        m_positionList[positionName] = positionPoint;
        m_positions.push_back(std::make_shared<Position>(Position(positionName.toStdString(), positionPoint, positionId)));
        positionCount++;
    }

    //find series from database
    QSqlQuery selectSerie("SELECT id, name, point FROM yoga_point WHERE is_serie = 1");
    while (selectSerie.next()) {
        int serieId = selectSerie.value(0).toInt();
        QString serieName = selectSerie.value(1).toString();
        std::vector<std::shared_ptr<YogaPoint>> yogaPoints;
        QSqlQuery selectSeriePosition("SELECT yoga_point_id FROM series WHERE id = ?");
        selectSeriePosition.addBindValue(serieId);
        if (!selectSeriePosition.exec()) {
            QMessageBox::critical(this, tr("Database error"), selectSeriePosition.lastError().text());
        }
        while (selectSeriePosition.next()) {
            //use position pointer from m_positions
            int positionId = selectSeriePosition.value(0).toInt();
            for (std::shared_ptr<YogaPoint> position : m_positions) {
                if (position->id() == positionId) {
                    yogaPoints.push_back(position);
                }
            }
        }
        m_positions.push_back(std::make_shared<Serie>(Serie(serieName.toStdString(), yogaPoints, serieId)));
    }

    QStringList positionNames;
    for (std::shared_ptr<YogaPoint> yogaPoint : m_positions) {
        positionNames << QString::fromStdString(yogaPoint->name());
    }
    m_addPositionComboBox->addItems(positionNames);
    m_addPositionComboBox->insertSeparator(positionCount);
}

void PositionView::addPosition()
{
    int rowCount = m_positionTable->rowCount();
    int row = rowCount;
    m_positionTable->setRowCount(row + 1);

    QString positionName = m_addPositionComboBox->currentText();
    QTableWidgetItem *positionNameItem = new QTableWidgetItem(positionName);
    m_positionTable->setItem(row, 0, positionNameItem);

    int times = m_addPositionSpinBox->value();
    QTableWidgetItem *timesItem = new QTableWidgetItem(QString::number(times));
    m_positionTable->setItem(row, 1, timesItem);

    QTableWidgetItem *trashIcon = new QTableWidgetItem(QIcon(":/images/trash.svg"), "");
    m_positionTable->setItem(row, 3, trashIcon);
}

void PositionView::positionChanged(const QString &text)
{
    for (auto it = m_positions.begin(); it != m_positions.end(); it++) {
        if ((*it)->name() == text.toStdString()) {
            m_timesPointLabel->setText(tr("x %1 points").arg((*it)->calculatePoints()));
        }
    }
}

void PositionView::positionTableCellClicked(int row, int column)
{
    if (column == 3) {
        QString positionName = m_positionTable->item(row, 0)->text();
        int ret = QMessageBox::warning(this, "",
            tr("Are you sure you want to delete the position %1?")
                .arg(positionName),
            QMessageBox::Yes | QMessageBox::Cancel
        );
        switch (ret) {
        case QMessageBox::Yes:
            m_positionTable->removeRow(row);
            break;
        default:
            break;
        }
    }
}

void PositionView::updatePoints(int row, int column)
{
    //to avoid issue with an infinite loop caused by "updatePoints"
    //we remove the connection during the update action
    disconnect(m_positionTable, SIGNAL(cellChanged(int,int)), this, SLOT(updatePoints(int, int)));

    //Calculate points for each row
    int sum = 0;
    for (int i = 0; i < m_positionTable->rowCount(); i++) {
        QTableWidgetItem *positionNameItem = m_positionTable->item(i, 0);
        QTableWidgetItem *timesItem = m_positionTable->item(i, 1);
        QTableWidgetItem *trashItem = m_positionTable->item(i, 3);
        if (positionNameItem && timesItem && trashItem) {
            QString positionName = m_positionTable->item(i, 0)->text();
            QString times = m_positionTable->item(i, 1)->text();
            int points = 0;
            for (std::shared_ptr<YogaPoint> position : m_positions) {
                if (QString::fromStdString(position->name()) == positionName) {
                    points = position->calculatePoints() * times.toInt();
                }
            }
            QTableWidgetItem *pointsItem = new QTableWidgetItem(QString::number(points));
            m_positionTable->setItem(i, 2, pointsItem);
            sum += points;
        }
    }
    m_pointLabel->setText(tr("Points: %1").arg(sum));

    connect(m_positionTable, SIGNAL(cellChanged(int,int)), this, SLOT(updatePoints(int, int)));
}

void PositionView::validateDailyPositions()
{
    QDate selectedDay = m_calendar->selectedDate();
    QSqlQuery deleteDailyPositions;
    if (!deleteDailyPositions.prepare("DELETE FROM daily_positions WHERE days = ?")) {
        QMessageBox::critical(this, tr("Database error"), deleteDailyPositions.lastError().text());
    }
    deleteDailyPositions.addBindValue(selectedDay);
    if (!deleteDailyPositions.exec()) {
        QMessageBox::critical(this, tr("Database error"), deleteDailyPositions.lastError().text());
    }

    for (int i = 0; i < m_positionTable->rowCount(); i++) {
        QTableWidgetItem *positionNameItem = m_positionTable->item(i, 0);
        QTableWidgetItem *timesItem = m_positionTable->item(i, 1);
        if (positionNameItem && timesItem) {
            std::string positionName = m_positionTable->item(i, 0)->text().toStdString();

            try {
                bool isSerie = YogaPoint::isSerie(positionName);
                std::unique_ptr<YogaPoint> yogaPoint = 0;
                if (isSerie) {
                    //TODO C++14 Replace double memory allocation by std::make_unique<Serie>
                    yogaPoint = std::unique_ptr<YogaPoint>(new Serie(positionName, m_positions));
                } else {
                    //TODO C++14 Replace double memory allocation by std::make_unique<Position>
                    yogaPoint = std::unique_ptr<YogaPoint>(new Position(Position::positionFromDatabase(positionName)));
                }
                QString times = m_positionTable->item(i, 1)->text();
                QSqlQuery insertDailyPositions;
                if (insertDailyPositions.prepare("INSERT INTO daily_positions VALUES (?, ?, ?, ?)")) {
                    insertDailyPositions.addBindValue(selectedDay);
                    insertDailyPositions.addBindValue(yogaPoint->id());
                    insertDailyPositions.addBindValue(times);
                    insertDailyPositions.addBindValue(isSerie);
                    if (!insertDailyPositions.exec()) {
                        throw std::runtime_error(insertDailyPositions.lastError().text().toStdString());
                    }
                } else {
                    throw std::runtime_error(insertDailyPositions.lastError().text().toStdString());
                }
            } catch (std::exception e) {
                QMessageBox::critical(this, tr("Error"), e.what());
            }
        }
    }
    //TODO feedback needed
}

void PositionView::dateSelected(const QDate &date)
{
    m_positionTable->clearContents();
    m_positionTable->setRowCount(0);
    int sum = 0;
    QSqlQuery selectDailyPositions("SELECT position_id, times, is_serie FROM daily_positions WHERE days = ?");
    selectDailyPositions.addBindValue(date);
    if (!selectDailyPositions.exec()) {
      QMessageBox::critical(this, tr("Database error"), selectDailyPositions.lastError().text());
    }

    int row = 0;
    while (selectDailyPositions.next()) {
        int positionId = selectDailyPositions.value(0).toInt();
        int times = selectDailyPositions.value(1).toInt();
        bool isSerie = selectDailyPositions.value(2).toBool();
        m_positionTable->setRowCount(row + 1);
        std::unique_ptr<YogaPoint> yogaPoint = 0;
        if (isSerie) {
            //TODO C++14 Replace double memory allocation by std::make_unique<Serie>
            yogaPoint = std::unique_ptr<YogaPoint>(new Serie(Serie::serieFromDatabase(positionId, m_positions)));
        } else {
            //TODO C++14 Replace double memory allocation by std::make_unique<Position>
            yogaPoint = std::unique_ptr<YogaPoint>(new Position(Position::positionFromDatabase(positionId)));
        }
        QTableWidgetItem *positionNameItem = new QTableWidgetItem(QString::fromStdString(yogaPoint->name()));
        m_positionTable->setItem(row, 0, positionNameItem);

        QTableWidgetItem *timesItem = new QTableWidgetItem(QString::number(times));
        m_positionTable->setItem(row, 1, timesItem);

        QTableWidgetItem *trashIcon = new QTableWidgetItem(QIcon(":/images/trash.svg"), "");
        m_positionTable->setItem(row, 3, trashIcon);

        sum += yogaPoint->calculatePoints() * times;

        row++;
    }
    m_pointLabel->setText(tr("Points: %1").arg(sum));
}

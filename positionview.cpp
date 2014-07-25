#include "positionview.h"
#include "mainwindow.h"
#include <QSqlQuery>
#include <QSqlError>

PositionView::PositionView(QWidget *parent) :
    QWidget(parent)
{
    //find positions from database
    QSqlQuery selectPosition("SELECT id, name, point FROM positions");
    while (selectPosition.next()) {
        int positionId = selectPosition.value(0).toInt();
        QString positionName = selectPosition.value(1).toString();
        int positionPoint = selectPosition.value(2).toInt();
        m_positionList[positionName] = positionPoint;
        m_positions.append(new Position(positionId, positionName, positionPoint));
    }

    //series
    QSqlQuery selectSerie("SELECT id, name FROM series GROUP BY id");
    while (selectSerie.next()) {
        int serieId = selectSerie.value(0).toInt();
        QString serieName = selectSerie.value(1).toString();
        QList<YogaPoint*> seriePositionList;
        QSqlQuery selectSeriePosition("SELECT position_id FROM series WHERE id = ?");
        selectSeriePosition.addBindValue(serieId);
        if (!selectSeriePosition.exec()) {
            QMessageBox::critical(this, tr("Database error"), selectSeriePosition.lastError().text());
        }
        while (selectSeriePosition.next()) {
            //use position pointer from m_positions
            int positionId = selectSeriePosition.value(0).toInt();
            for (YogaPoint* position : m_positions) {
                if (position->id() == positionId) {
                    seriePositionList.append(position);
                }
            }
        }
        qDebug() << serieName;
        m_positions.append(new Serie(serieId, serieName, seriePositionList));
    }
    /* DEBUG */
    qDebug() << endl << endl;
    for (YogaPoint* yogaPoint : m_positions) {
        qDebug() << "Name: " << yogaPoint->name() << ", Points: " << yogaPoint->calculatePoints();
    }

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QLabel *title = new QLabel(tr("Position"));
    m_positionTable = new QTableWidget(0, 4);
    m_positionTable->setHorizontalHeaderLabels(QStringList(tr("Position")) << tr("Times") << tr("Points") << "");
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
    QStringList positionNames;
    for (auto it = m_positions.begin(); it != m_positions.end(); it++) {
        positionNames << (*it)->name();
    }
    m_addPositionComboBox->addItems(positionNames);

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


    //a signal mapper will be usefull for delete button of each position/serie
    m_signalMapper = new QSignalMapper(this);

    connect(addPositionButton, SIGNAL(clicked()), this, SLOT(addPosition()));
    connect(m_addPositionComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(positionChanged(QString)));
    connect(m_positionTable, SIGNAL(cellClicked(int,int)), this, SLOT(positionTableCellClicked(int,int)));
    connect(m_positionTable, SIGNAL(cellChanged(int,int)), this, SLOT(updatePoints(int, int)));
    connect(m_validateButton, SIGNAL(clicked()), this, SLOT(validateDailyPositions()));
    connect(m_calendar, SIGNAL(clicked(QDate)), this, SLOT(dateSelected(QDate)));

    positionChanged(m_positions.first()->name());
}

PositionView::~PositionView()
{
    std::for_each(m_positions.begin(), m_positions.end(), [](YogaPoint* position) { delete position; });
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

    QTableWidgetItem *trashIcon = new QTableWidgetItem(QIcon(":/trash.svg"), "");
    m_positionTable->setItem(row, 3, trashIcon);
}

void PositionView::positionChanged(const QString &text)
{
    for (auto it = m_positions.begin(); it != m_positions.end(); it++) {
        if ((*it)->name() == text) {
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
            for (auto it = m_positions.begin(); it != m_positions.end(); it++) {
                if ((*it)->name() == positionName) {
                    points = (*it)->calculatePoints() * times.toInt();
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
            QString positionName = m_positionTable->item(i, 0)->text();
            Position position = Position::positionFromDatabase(positionName, this);
            QString times = m_positionTable->item(i, 1)->text();
            QSqlQuery insertDailyPositions;
            if (insertDailyPositions.prepare("INSERT INTO daily_positions VALUES (?, ?, ?)")) {
                insertDailyPositions.addBindValue(selectedDay);
                insertDailyPositions.addBindValue(position.id());
                insertDailyPositions.addBindValue(times);
                if (!insertDailyPositions.exec()) {
                    QMessageBox::critical(this, tr("Database error"), insertDailyPositions.lastError().text());
                }
            } else {
                QMessageBox::critical(this, tr("Database error"), insertDailyPositions.lastError().text());
            }
        }
    }
}

void PositionView::dateSelected(const QDate &date)
{
    m_positionTable->clearContents();
    m_positionTable->setRowCount(0);
    QSqlQuery selectDailyPositions("SELECT position_id, times FROM daily_positions WHERE days = ?");
    selectDailyPositions.addBindValue(date);
    if (!selectDailyPositions.exec()) {
      QMessageBox::critical(this, tr("Database error"), selectDailyPositions.lastError().text());
    }

    int row = 0;
    while (selectDailyPositions.next()) {
        int positionId = selectDailyPositions.value(0).toInt();
        int times = selectDailyPositions.value(1).toInt();
        m_positionTable->setRowCount(row + 1);
        Position position = Position::positionFromDatabase(positionId, this);
        QTableWidgetItem *positionNameItem = new QTableWidgetItem(position.name());
        m_positionTable->setItem(row, 0, positionNameItem);

        QTableWidgetItem *timesItem = new QTableWidgetItem(QString::number(times));
        m_positionTable->setItem(row, 1, timesItem);

        QTableWidgetItem *trashIcon = new QTableWidgetItem(QIcon(":/trash.svg"), "");
        m_positionTable->setItem(row, 3, trashIcon);

        row++;
    }
}

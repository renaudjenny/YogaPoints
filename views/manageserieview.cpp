#include "manageserieview.h"
#include <QBoxLayout>
#include <QLabel>
#include <QSql>
#include <QHeaderView>

ManageSerieView::ManageSerieView(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Manage series"));

    //create widgets
    QLabel* seriesLabel = new QLabel(tr("Series"));
    m_serieComboBox = new QComboBox;
    m_positionsTable = new QTableWidget;
    m_positionsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_positionsTable->setColumnCount(3);
    QStringList positionTableHeaderLabels = QStringList("id (hidden)") << tr("Name") << tr("Points");
    m_positionsTable->setColumnHidden(0, true);
    m_positionsTable->setHorizontalHeaderLabels(positionTableHeaderLabels);
    m_positionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_yogaPointComboBox = new QComboBox;
    m_addPositionButton = new QPushButton(tr("Add Position/Serie"));
    m_removePositionButton = new QPushButton(tr("Remove Position/Serie"));
    m_removePositionButton->setEnabled(false);
    m_validateButton = new QPushButton(tr("Validate"));
    m_validateButton->setEnabled(false);
    m_cancelButton = new QPushButton(tr("Cancel"));
    m_closeButton = new QPushButton(tr("Close"));

    //Place widgets
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QHBoxLayout* serieLayout = new QHBoxLayout;
    serieLayout->addWidget(seriesLabel);
    serieLayout->addWidget(m_serieComboBox);
    serieLayout->addStretch();
    mainLayout->addLayout(serieLayout);

    QHBoxLayout* positionsLayout = new QHBoxLayout;
    positionsLayout->addWidget(m_positionsTable);
    QVBoxLayout* positionButtonsLayout = new QVBoxLayout;
    positionButtonsLayout->addStretch();
    QHBoxLayout* addPositionLayout = new QHBoxLayout;
    addPositionLayout->addWidget(m_addPositionButton);
    addPositionLayout->addWidget(m_yogaPointComboBox);
    positionButtonsLayout->addLayout(addPositionLayout);
    positionButtonsLayout->addWidget(m_removePositionButton);
    positionsLayout->addLayout(positionButtonsLayout);
    mainLayout->addLayout(positionsLayout);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_closeButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(m_cancelButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(m_validateButton, 0, Qt::AlignRight);
    mainLayout->addLayout(buttonsLayout);

    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_serieComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(serieSelected(int)));
    connect(m_positionsTable, SIGNAL(cellClicked(int,int)), this, SLOT(positionClicked(int,int)));
    connect(m_removePositionButton, SIGNAL(clicked()), this, SLOT(removePosition()));
    connect(m_addPositionButton, SIGNAL(clicked()), this, SLOT(addPosition()));
    connect(m_validateButton, SIGNAL(clicked()), this, SLOT(validate()));

    populateYogaPoint();
    populateSerieComboBox();
    populatePositionTable(m_originalSeries.first());
}

ManageSerieView::~ManageSerieView()
{
}

void ManageSerieView::populateYogaPoint()
{
    QSqlQuery query("SELECT name, point, id FROM yoga_point");
    while (query.next()) {
        m_availableYogaPoint.push_back(
            std::make_shared<Position>(Position(query.value(0).toString().toStdString(), query.value(1).toInt(), query.value(2).toInt()))
        );
    }
    for (std::shared_ptr<YogaPoint> yogaPoint : m_availableYogaPoint) {
        m_yogaPointComboBox->addItem(QString::fromStdString(yogaPoint->name()));
    }
}

void ManageSerieView::populateSerieComboBox()
{
    QSqlQuery query("SELECT name FROM yoga_point WHERE is_serie = 1");
    while (query.next()) {
        QString serieName = query.value(0).toString();
        m_serieComboBox->addItem(serieName);
        m_originalSeries.append(Serie::serieFromDatabase(serieName.toStdString(), m_availableYogaPoint));
    }
}

void ManageSerieView::populatePositionTable(const Serie& serie)
{
    std::vector<std::shared_ptr<YogaPoint>> yogaPoints = serie.getYogaPoints();
    m_positionsTable->setRowCount(yogaPoints.size());
    for (int i = 0; i < yogaPoints.size(); i++) {
        std::shared_ptr<YogaPoint> yogaPoint = yogaPoints[i];
        QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(yogaPoint->name()));
        m_positionsTable->setItem(i, 1, nameItem);
        QTableWidgetItem* pointItem = new QTableWidgetItem(QString::number(yogaPoint->calculatePoints()));
        m_positionsTable->setItem(i, 2, pointItem);
    }
}

void ManageSerieView::addPosition()
{
    std::string yogaPointName = m_yogaPointComboBox->currentText().toStdString();
    std::unique_ptr<YogaPoint> yogaPoint = 0;
    if (YogaPoint::isSerie(yogaPointName)) {
        //TODO C++14 Replace double memory allocation by std::make_unique<Serie>
        yogaPoint = std::unique_ptr<Serie>(new Serie(Serie::serieFromDatabase(yogaPointName, m_availableYogaPoint)));
    } else {
        //TODO C++14 Replace double memory allocation by std::make_unique<Position>
        yogaPoint = std::unique_ptr<Position>(new Position(Position::positionFromDatabase(yogaPointName)));
    }
    int id = yogaPoint->id();
    int points = yogaPoint->calculatePoints();
    int rowCount = m_positionsTable->rowCount();
    m_positionsTable->setRowCount(rowCount + 1);
    m_positionsTable->setItem(rowCount, 0, new QTableWidgetItem(QString::number(id)));
    m_positionsTable->setItem(rowCount, 1, new QTableWidgetItem(QString::fromStdString(yogaPointName)));
    m_positionsTable->setItem(rowCount, 2, new QTableWidgetItem(QString::number(points)));

    m_validateButton->setEnabled(true);
}

void ManageSerieView::validate()
{


    QMessageBox::information(this, tr("Update position"), tr("Series are successfuly updated in database."));
}

void ManageSerieView::serieSelected(int index)
{
    if (m_lastSelectedSerieIndex != index) {
        //Check if the serie is currently edited, in this case prompt to warn user that modification will be lost
        bool readyToChangeSerie = !m_validateButton->isEnabled();
        if (!readyToChangeSerie) {
            readyToChangeSerie = QMessageBox::Ok == QMessageBox::warning(this, tr("This serie has some unsaved modifications"),
                tr("if you change serie without validate the previous one, modification will be lost"),
                QMessageBox::Ok, QMessageBox::Cancel);
        }
        if (readyToChangeSerie) {
            QString serieName = m_serieComboBox->itemText(index);
            QList<Serie>::iterator it = std::find_if(m_originalSeries.begin(), m_originalSeries.end(), [&serieName](const Serie& serie)
            {
                return QString::fromStdString(serie.name()) == serieName;
            });
            if (it != m_originalSeries.end()) {
                populatePositionTable(*it);
            }
            m_validateButton->setEnabled(false);
            m_lastSelectedSerieIndex = index;
        } else {
            m_serieComboBox->setCurrentIndex(m_lastSelectedSerieIndex);
        }
    }
}

void ManageSerieView::positionClicked(int row, int column)
{
    m_removePositionButton->setEnabled(true);
    m_positionsTable->setRangeSelected(QTableWidgetSelectionRange(row, 0, row, 2), true);
}

void ManageSerieView::removePosition()
{
    int selectedPositionRow = m_positionsTable->currentRow();
    m_positionsTable->removeRow(selectedPositionRow);
    m_validateButton->setEnabled(true);
}

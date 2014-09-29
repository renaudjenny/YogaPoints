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
    m_positionsTable->setColumnCount(2);
    QStringList positionTableHeaderLabels = QStringList("Name") << tr("Points");
    m_positionsTable->setHorizontalHeaderLabels(positionTableHeaderLabels);
    m_positionsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_yogaPointComboBox = new QComboBox;
    m_addPositionButton = new QPushButton(tr("Add Position"));
    m_removePositionButton = new QPushButton(tr("Remove Position"));
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
    addPositionLayout->addWidget(m_yogaPointComboBox);
    addPositionLayout->addWidget(m_addPositionButton);
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
            std::shared_ptr<YogaPoint>(new Position(query.value(0).toString().toStdString(), query.value(1).toInt(), query.value(2).toInt()))
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
        m_positionsTable->setItem(i, 0, nameItem);
        QTableWidgetItem* pointItem = new QTableWidgetItem(QString::number(yogaPoint->calculatePoints()));
        m_positionsTable->setItem(i, 1, pointItem);
    }
}

void ManageSerieView::setPositionOnTable(const Position &position, int row)
{
    //TODO
}

void ManageSerieView::serieSelected(int index)
{
    QString serieName = m_serieComboBox->itemText(index);
    QList<Serie>::iterator it = std::find_if(m_originalSeries.begin(), m_originalSeries.end(), [&serieName](const Serie& serie)
    {
        return QString::fromStdString(serie.name()) == serieName;
    });
    if (it != m_originalSeries.end()) {
        populatePositionTable(*it);
    }
}

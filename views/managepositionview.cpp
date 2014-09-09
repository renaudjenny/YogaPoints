#include "managepositionview.h"
#include <QHeaderView>
#include "addpositionview.h"
#include <algorithm>

ManagePositionView::ManagePositionView(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Manage positions and series"));

    //Create widgets
    m_positionTable = new QTableWidget;
    m_validateButton = new QPushButton(tr("Validate"));
    m_validateButton->setEnabled(false);
    m_cancelButton = new QPushButton(tr("Cancel"));
    m_closeButton = new QPushButton(tr("Close"));
    m_addNewPositionButton = new QPushButton(tr("Add new position"));
    m_deletePositionButton = new QPushButton(tr("Delete position"));
    m_deletePositionButton->setEnabled(false);

    //set position table
    m_positionTable->setSortingEnabled(false);
    m_positionTable->setColumnCount(3);
    //keep id reference but hide it
    m_positionTable->hideColumn(0);
    QStringList positionTableHeaderLabels = QStringList("id (hidden)") << tr("Name") << tr("Points");
    m_positionTable->setHorizontalHeaderLabels(positionTableHeaderLabels);
    m_positionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Place widgets
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QHBoxLayout* tableLayout = new QHBoxLayout;
    tableLayout->addWidget(m_positionTable);
    QVBoxLayout* manageButtonsLayout = new QVBoxLayout;
    manageButtonsLayout->addStretch();
    manageButtonsLayout->addWidget(m_addNewPositionButton);
    manageButtonsLayout->addWidget(m_deletePositionButton);
    tableLayout->addLayout(manageButtonsLayout);
    mainLayout->addLayout(tableLayout);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_closeButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(m_cancelButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(m_validateButton, 0, Qt::AlignRight);
    mainLayout->addLayout(buttonsLayout);

    //populate postions
    populatePositionTable();

    //Signal and slot connections
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(m_addNewPositionButton, SIGNAL(clicked()), this, SLOT(addNewPosition()));
    connect(m_deletePositionButton, SIGNAL(clicked()), this, SLOT(deletePosition()));
    connect(m_positionTable, SIGNAL(cellClicked(int,int)), this, SLOT(positionClicked(int,int)));
    connect(m_positionTable, SIGNAL(cellChanged(int,int)), this, SLOT(positionChanged(int, int)));
    connect(m_validateButton, SIGNAL(clicked()), this, SLOT(validate()));
}

ManagePositionView::~ManagePositionView()
{
}

void ManagePositionView::populatePositionTable()
{
    //reset
    m_positionTable->clearContents();
    m_originalPositions.clear();
    m_newPositions.clear();

    QSqlQuery query = QSqlQuery("SELECT id, name, point FROM positions ORDER BY id");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        int point = query.value(2).toInt();
        Position position(name, point, id);
        m_originalPositions.append(position);
        m_newPositions.append(position);
    }

    //populate the table
    m_positionTable->setRowCount(m_originalPositions.count());
    for (int i = 0; i < m_originalPositions.count(); i++) {
        Position position = m_originalPositions[i];
        setPositionOnTable(position, i);
    }
}

void ManagePositionView::setPositionOnTable(const Position& position, int row)
{
    QTableWidgetItem* positionId = new QTableWidgetItem(QString::number(position.id()));
    m_positionTable->setItem(row, 0, positionId);
    QTableWidgetItem* positionName = new QTableWidgetItem(position.name());
    m_positionTable->setItem(row, 1, positionName);
    QTableWidgetItem* positionPoint = new QTableWidgetItem(QString::number(position.calculatePoints()));
    m_positionTable->setItem(row, 2, positionPoint);
}

void ManagePositionView::validate()
{
    for (Position position : m_newPositions) {
        position.save(this);
    }

    QList<Position> positionsToDelete;
    std::set_difference(m_originalPositions.begin(), m_originalPositions.end(), m_newPositions.begin(), m_newPositions.end(), std::inserter(positionsToDelete, positionsToDelete.begin()));

    for (Position positionToDelete : positionsToDelete) {
        positionToDelete.deleteFromDB(this);
    }

    //display this sentence only if there is at least one position deleted
    QString deletedPositionInformation = positionsToDelete.count() > 0 ? tr(" There are %n positions deleted.", "", positionsToDelete.count()) : "";
    //display this sentence only if there is at least one position added
    QList<Position> positionsAdded;
    std::set_difference(m_newPositions.begin(), m_newPositions.end(), m_originalPositions.begin(), m_originalPositions.end(), std::inserter(positionsAdded, positionsAdded.begin()));
    QString addedPositionInformation = positionsAdded.size() > 0 ? tr(" There are %n positions added.", "", positionsAdded.size()) : "";
    QMessageBox::information(this, tr("Update position"), tr("Positions are successfuly updated in database.") + deletedPositionInformation + addedPositionInformation);

    m_originalPositions = m_newPositions;
}

void ManagePositionView::addNewPosition()
{
    AddPositionView* addPositionView = new AddPositionView(this);
    addPositionView->setModal(true);
    if (addPositionView->exec() == QDialog::Accepted) {
        Position position(addPositionView->positionName(), addPositionView->point());
        m_newPositions.append(position);
        m_positionTable->setRowCount(m_positionTable->rowCount() + 1);
        setPositionOnTable(position, m_newPositions.count() - 1);
        m_validateButton->setEnabled(true);
    }
}

void ManagePositionView::deletePosition()
{
    m_positionTable->removeRow(m_selectedPositionRow);
    //put position to remove on positionsToDelete list
    m_newPositions.removeAt(m_selectedPositionRow);
    m_validateButton->setEnabled(true);
}

void ManagePositionView::positionClicked(int row, int column)
{
    m_deletePositionButton->setEnabled(true);
    m_selectedPositionRow = row;
}

void ManagePositionView::positionChanged(int row, int column)
{
    //put modification
    QTableWidgetItem* positionId = m_positionTable->item(row, 0);
    QTableWidgetItem* positionName = m_positionTable->item(row, 1);
    QTableWidgetItem* positionPoint = m_positionTable->item(row, 2);
    if (positionId && positionName && positionPoint) {
        m_validateButton->setEnabled(true);
        if (m_newPositions.size() > row && m_newPositions[row].id() == positionId->text().toInt()) {
            m_newPositions[row].setName(positionName->text());
            m_newPositions[row].setPoint(positionPoint->text().toInt());
        }
    }
}

void ManagePositionView::cancel()
{
    populatePositionTable();
}


#include "managepositionview.h"
#include <QHeaderView>

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
    std::for_each(m_positions.begin(), m_positions.end(), [](Position* position) { delete position; });
}

void ManagePositionView::populatePositionTable()
{
    //reset
    m_positionTable->clearContents();
    m_positions.clear();

    QSqlQuery query = QSqlQuery("SELECT id, name, point FROM positions ORDER BY id");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        int point = query.value(2).toInt();
        Position* position = new Position(name, point, id);
        m_positions.append(position);
    }

    //populate the table
    //disconnect SIGNAL/SLOT to prevent firing cellChanged on table
    disconnect(m_positionTable, SIGNAL(cellChanged(int,int)), this, SLOT(positionChanged(int, int)));
    m_positionTable->setRowCount(m_positions.count());
    for (int i = 0; i < m_positions.count(); i++) {
        Position* position = m_positions[i];
        QTableWidgetItem* positionId = new QTableWidgetItem(QString::number(position->id()));
        m_positionTable->setItem(i, 0, positionId);
        QTableWidgetItem* positionName = new QTableWidgetItem(position->name());
        m_positionTable->setItem(i, 1, positionName);
        QTableWidgetItem* positionPoint = new QTableWidgetItem(QString::number(position->calculatePoints()));
        m_positionTable->setItem(i, 2, positionPoint);
    }
    //job is done, reconnect SIGNAL/SLOT
    connect(m_positionTable, SIGNAL(cellChanged(int,int)), this, SLOT(positionChanged(int, int)));
}

void ManagePositionView::validate()
{
    for (Position* position : m_positions) {
        position->save(this);
    }
    for (Position* positionToDelete : m_positionsToDelete) {
        positionToDelete->deleteFromDB(this);
    }
    //display this sentence only if there is at least one position deleted
    QString deletedPositionInformation = m_positionsToDelete.count() > 0 ? tr(" There are %n positions deleted", "", m_positionsToDelete.count()) : "";
    QMessageBox::information(this, tr("Update position"), tr("Positions are successfuly updated in database.") + deletedPositionInformation);
}

void ManagePositionView::addNewPosition()
{
    m_positionTable->setRowCount(m_positionTable->rowCount() + 1);
    m_validateButton->setEnabled(true);
}

void ManagePositionView::deletePosition()
{
    m_positionTable->removeRow(m_selectedPositionRow);
    //put position to remove on positionsToDelete list
    m_positionsToDelete.append(m_positions[m_selectedPositionRow]);
    m_positions.removeAt(m_selectedPositionRow);
    m_validateButton->setEnabled(true);
}

void ManagePositionView::positionClicked(int row, int column)
{
    m_deletePositionButton->setEnabled(true);
    m_selectedPositionRow = row;
}

void ManagePositionView::positionChanged(int row, int column)
{
    m_validateButton->setEnabled(true);

    //put modification
    QTableWidgetItem* positionId = m_positionTable->item(row, 0);
    QTableWidgetItem* positionName = m_positionTable->item(row, 1);
    QTableWidgetItem* positionPoint = m_positionTable->item(row, 2);

    if (m_positions[row]->id() == positionId->text().toInt()) {
        m_positions[row]->setName(positionName->text());
        m_positions[row]->setPoint(positionPoint->text().toInt());
    }
}

void ManagePositionView::cancel()
{
    populatePositionTable();
}


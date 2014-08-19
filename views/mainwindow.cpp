#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    DatabaseManager databaseManager;
    //Move all code below in a method (static... or not) of the databaseManager
    databaseManager.createDatabaseSchema(this);

    m_positionView = new PositionView(this);
    setCentralWidget(m_positionView);

    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{
    DatabaseManager db;
    db.deleteDB();
}

void MainWindow::createActions()
{
    addPositionAct = new QAction(tr("&Add a position"), this);
    addPositionAct->setShortcut(QKeySequence::New);
    addPositionAct->setToolTip(tr("Add a new position in the list"));
    connect(addPositionAct, SIGNAL(triggered()), this, SLOT(openAddPositionView()));
}

void MainWindow::createMenus()
{
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(addPositionAct);
}

void MainWindow::openAddPositionView()
{
    AddPositionView* addPositionView = new AddPositionView;
    if (addPositionView->exec() == QDialog::Accepted) {
        //TODO emit a signal
    }
}

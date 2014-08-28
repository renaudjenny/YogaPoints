#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    DatabaseManager databaseManager;
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
    managePositionAct = new QAction(tr("&Manage positions"), this);
    managePositionAct->setShortcut(QKeySequence::Print);
    managePositionAct->setToolTip(tr("Manage positions, create new one, edit or delete"));
    connect(managePositionAct, SIGNAL(triggered()), this, SLOT(openManagePositionView()));
}

void MainWindow::createMenus()
{
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(managePositionAct);
}

void MainWindow::openManagePositionView()
{
    ManagePositionView* managePositionView = new ManagePositionView;
    if (managePositionView->exec() == QDialog::Accepted) {
        m_positionView->updatePositions();
    }
}

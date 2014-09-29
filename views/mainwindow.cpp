#include "mainwindow.h"
#include "manageserieview.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    DatabaseManager databaseManager;
    try {
        databaseManager.createDatabaseSchema();
    } catch (std::exception e) {
        QMessageBox::critical(this, tr("Database error"), e.what());
    }

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
    m_managePositionAct = new QAction(tr("&Manage positions"), this);
    m_managePositionAct->setToolTip(tr("Manage positions, create new one, edit or delete"));
    connect(m_managePositionAct, SIGNAL(triggered()), this, SLOT(openManagePositionView()));

    m_manageSerieAct = new QAction(tr("&Manage series"), this);
    m_manageSerieAct->setToolTip(tr("Manage series, create new one, edit or delete"));
    connect(m_manageSerieAct, SIGNAL(triggered()), this, SLOT(openManageSerieView()));
}

void MainWindow::createMenus()
{
    m_editMenu = menuBar()->addMenu(tr("&Edit"));
    m_editMenu->addAction(m_managePositionAct);
    m_editMenu->addAction(m_manageSerieAct);
}

void MainWindow::openManagePositionView()
{
    ManagePositionView* managePositionView = new ManagePositionView(this);
    managePositionView->exec();
    m_positionView->updatePositions();
}

void MainWindow::openManageSerieView()
{
    ManageSerieView* manageSerieView = new ManageSerieView(this);
    manageSerieView->exec();
    m_positionView->updatePositions();
}

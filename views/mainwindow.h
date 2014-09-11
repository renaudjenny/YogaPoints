#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "views/positionview.h"
#include "views/addpointsview.h"
#include "views/managepositionview.h"
#include "managers/databasemanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QMenu* m_editMenu;
    QAction* m_managePositionAct;
    QAction* m_manageSerieAct;
    PositionView *m_positionView;
    void createActions();
    void createMenus();
signals:
public slots:
    void openManagePositionView();
    void openManageSerieView();
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "addpointsview.h"
#include "positionview.h"
#include "addpositionview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QMenu* editMenu;
    QAction* addPositionAct;
    PositionView *m_positionView;
    void createActions();
    void createMenus();
signals:
public slots:
    void openAddPositionView();
};

#endif // MAINWINDOW_H

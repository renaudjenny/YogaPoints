#ifndef ADDPOINTSVIEW_H
#define ADDPOINTSVIEW_H

#include <QWidget>
#include <QtWidgets>
#include "views/positionview.h"

class AddPointsView : public QWidget
{
    Q_OBJECT
public:
    explicit AddPointsView(QWidget *parent = 0);
    const QPushButton *getPositionsButton();
    const QPushButton *getSeriesButton();
private:
    QPushButton *m_positionsButton;
    QPushButton *m_seriesButton;
signals:

public slots:

};

#endif // ADDPOINTSVIEW_H

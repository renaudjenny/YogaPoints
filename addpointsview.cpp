#include "addpointsview.h"
#include "positionview.h"

AddPointsView::AddPointsView(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QLabel *title = new QLabel(tr("Ajouter des points"));
    m_positionsButton = new QPushButton(tr("Positions"));
    m_seriesButton = new QPushButton(tr("Series"));

    setLayout(mainLayout);
    mainLayout->addWidget(title);
    mainLayout->addWidget(m_positionsButton);
    mainLayout->addWidget(m_seriesButton);
}

const QPushButton *AddPointsView::getPositionsButton()
{
    return m_positionsButton;
}

const QPushButton *AddPointsView::getSeriesButton()
{
    return m_seriesButton;
}

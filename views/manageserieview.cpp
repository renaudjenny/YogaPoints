#include "manageserieview.h"
#include <QBoxLayout>
#include <QLabel>

ManageSerieView::ManageSerieView(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Manage series"));

    //create widgets
    QLabel* seriesLabel = new QLabel(tr("Series"));
    m_serieComboBox = new QComboBox;
    m_positionsTable = new QTableWidget;
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
    positionButtonsLayout->addWidget(m_addPositionButton);
    positionButtonsLayout->addWidget(m_removePositionButton);
    positionsLayout->addLayout(positionButtonsLayout);
    mainLayout->addLayout(positionsLayout);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_closeButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(m_cancelButton, 0, Qt::AlignRight);
    buttonsLayout->addWidget(m_validateButton, 0, Qt::AlignRight);
    mainLayout->addLayout(buttonsLayout);
}

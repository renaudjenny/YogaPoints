#include "addpositionview.h"

AddPositionView::AddPositionView(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("Add position"));

    //Create widgets
    m_nameEdit = new QLineEdit;
    m_pointSpinBox = new QSpinBox;
    m_addPositionButton = new QPushButton(tr("Add position"));

    //Place widgets
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QFormLayout* formLayout = new QFormLayout;
    mainLayout->addLayout(formLayout);
    formLayout->addRow(tr("Name"), m_nameEdit);
    formLayout->addRow(tr("Point"), m_pointSpinBox);

    mainLayout->addWidget(m_addPositionButton);

    //TODO Signal and slot connections

}

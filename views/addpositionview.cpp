#include "addpositionview.h"
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>

//TODO Don't let user click OK if position has no name or point is 0
AddPositionView::AddPositionView(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QFormLayout* formLayout = new QFormLayout;
    m_positionNameEdit = new QLineEdit;
    m_pointSpinBox = new QSpinBox;
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    QPushButton* okButton = new QPushButton(tr("Add position"));
    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    formLayout->addRow(tr("Position name"), m_positionNameEdit);
    formLayout->addRow(tr("Position point"), m_pointSpinBox);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

QString AddPositionView::positionName() const
{
    return m_positionNameEdit->text();
}

int AddPositionView::point()
{
    return m_pointSpinBox->value();
}

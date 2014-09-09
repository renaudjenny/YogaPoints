#include "addpositionview.h"
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include "positionview.h"

AddPositionView::AddPositionView(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QFormLayout* formLayout = new QFormLayout;
    m_positionNameEdit = new QLineEdit;
    m_pointSpinBox = new QSpinBox;
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    m_okButton = new QPushButton(tr("Add position"));
    m_okButton->setEnabled(false);
    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    formLayout->addRow(tr("Position name"), m_positionNameEdit);
    formLayout->addRow(tr("Position point"), m_pointSpinBox);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(m_okButton);
    buttonsLayout->addWidget(cancelButton);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    connect(m_okButton, SIGNAL(clicked()), this, SLOT(validateNewPosition()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(m_positionNameEdit, SIGNAL(textChanged(QString)), this, SLOT(checkValidity()));
    connect(m_pointSpinBox, SIGNAL(valueChanged(int)), this, SLOT(checkValidity()));
}

QString AddPositionView::positionName() const
{
    return m_positionNameEdit->text();
}

int AddPositionView::point()
{
    return m_pointSpinBox->value();
}

void AddPositionView::validateNewPosition()
{
    //check if position name already exist
    Position position = Position::positionFromDatabase(m_positionNameEdit->text(), this);
    if (position.id() > 0) {
        QMessageBox::information(this, tr("Position already exist"), tr("The position %1 already exist in database").arg(m_positionNameEdit->text()));
    } else {
        accept();
    }
}

void AddPositionView::checkValidity()
{
    m_okButton->setEnabled(!m_positionNameEdit->text().isEmpty() && m_pointSpinBox->value() > 0);
}

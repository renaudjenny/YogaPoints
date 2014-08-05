#include "addpositionview.h"

AddPositionView::AddPositionView(QWidget *parent) :
    QDialog(parent)
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

    //Signal and slot connections
    connect(m_addPositionButton, SIGNAL(clicked()), this, SLOT(validateNewPosition()));
}

void AddPositionView::validateNewPosition()
{
    if (!m_nameEdit->text().isEmpty() && m_pointSpinBox->value() > 0) {
        Position position(m_nameEdit->text(), m_pointSpinBox->value());
        position.save(this);
        accept();
        /*
        QSqlQuery query("INSERT INTO positions (name, point) VALUES (?, ?)");
        query.addBindValue(m_nameEdit->text());
        query.addBindValue(m_pointSpinBox->value());
        if (query.exec()) {
            accept();
        } else {
            QMessageBox::critical(this, tr("Database error"), query.lastError().text());
        }
        */
    } else {
        QMessageBox::warning(this, tr("Non valid arguments"), tr("You have to set a name and points (greather than 0) to add a position"));
    }
}

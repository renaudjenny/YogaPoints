#include "positionserieselector.h"

PositionSerieSelector::PositionSerieSelector(QWidget *parent) :
    QWidget(parent)
{
    initDefault();
}

QComboBox *PositionSerieSelector::getComboBox()
{
    return m_comboBox;
}

void PositionSerieSelector::setComboxBox(QComboBox *comboBox)
{
    m_comboBox = comboBox;
}

QSpinBox *PositionSerieSelector::getCounterSpinBox()
{
    return m_counterSpinBox;
}

void PositionSerieSelector::setCounterSpinBox(QSpinBox *counterSpinBox)
{
    m_counterSpinBox = counterSpinBox;
}

QIcon PositionSerieSelector::getTrashIcon()
{
    return m_trashIcon;
}

void PositionSerieSelector::setTrashIcon(QIcon trashIcon)
{
    m_trashIcon = trashIcon;
}

QPushButton *PositionSerieSelector::getTrashButton()
{
    return m_trashButton;
}

void PositionSerieSelector::initDefault()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    m_comboBox = new QComboBox;
    m_counterSpinBox = new QSpinBox;
    m_trashIcon = QIcon(":/trash.svg");
    m_trashButton = new QPushButton(m_trashIcon, "");

    setLayout(mainLayout);
    mainLayout->addWidget(m_comboBox);
    mainLayout->addWidget(m_counterSpinBox);
    mainLayout->addWidget(m_trashButton);
}

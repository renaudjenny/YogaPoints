#ifndef POSITIONSERIESELECTOR_H
#define POSITIONSERIESELECTOR_H

#include <QWidget>
#include <QtWidgets>

class PositionSerieSelector : public QWidget
{
    Q_OBJECT
public:
    explicit PositionSerieSelector(QWidget *parent = 0);
    QComboBox *getComboBox();
    void setComboxBox(QComboBox *comboBox);
    QSpinBox *getCounterSpinBox();
    void setCounterSpinBox(QSpinBox *counterSpinBox);
    QIcon getTrashIcon();
    void setTrashIcon(QIcon trashIcon);
    QPushButton *getTrashButton();
private:
    QComboBox *m_comboBox;
    QSpinBox *m_counterSpinBox;
    QIcon m_trashIcon;
    QPushButton *m_trashButton;

    void initDefault();

signals:

public slots:
};

#endif // POSITIONSERIESELECTOR_H

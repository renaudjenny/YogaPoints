#ifndef ADDPOSITIONVIEW_H
#define ADDPOSITIONVIEW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>

class AddPositionView : public QWidget
{
    Q_OBJECT
public:
    explicit AddPositionView(QWidget *parent = 0);
private:
    QLineEdit* m_nameEdit;
    QSpinBox* m_pointSpinBox;
    QPushButton* m_addPositionButton;
signals:

public slots:

};

#endif // ADDPOSITIONVIEW_H

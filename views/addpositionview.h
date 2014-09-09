#ifndef ADDPOSITIONVIEW_H
#define ADDPOSITIONVIEW_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

class AddPositionView : public QDialog
{
    Q_OBJECT
public:
    explicit AddPositionView(QWidget *parent = 0);
    QString positionName() const;
    int point();
private:
    QPushButton* m_okButton;
    QLineEdit* m_positionNameEdit;
    QSpinBox* m_pointSpinBox;
signals:

public slots:
    void validateNewPosition();
    void checkValidity();
};

#endif // ADDPOSITIONVIEW_H

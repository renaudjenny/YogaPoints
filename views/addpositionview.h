#ifndef ADDPOSITIONVIEW_H
#define ADDPOSITIONVIEW_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "models/position.h"
#include "models/serie.h"

class AddPositionView : public QDialog
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
    void validateNewPosition();
};

#endif // ADDPOSITIONVIEW_H

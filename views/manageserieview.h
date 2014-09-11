#ifndef MANAGESERIEVIEW_H
#define MANAGESERIEVIEW_H

#include <QWidget>
#include <QDialog>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>

class ManageSerieView : public QDialog
{
    Q_OBJECT
public:
    explicit ManageSerieView(QWidget *parent = 0);

private:
    QComboBox* m_serieComboBox;
    QTableWidget* m_positionsTable;
    QPushButton* m_addPositionButton;
    QPushButton* m_removePositionButton;
    QPushButton* m_validateButton;
    QPushButton* m_cancelButton;
    QPushButton* m_closeButton;
signals:

public slots:

};

#endif // MANAGESERIEVIEW_H

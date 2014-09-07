#ifndef MANAGEPOSITIONVIEW_H
#define MANAGEPOSITIONVIEW_H

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
#include <QTableWidget>
#include "models/position.h"

class ManagePositionView : public QDialog
{
    Q_OBJECT
public:
    explicit ManagePositionView(QWidget *parent = 0);
    ~ManagePositionView();
private:
    QTableWidget* m_positionTable;
    int m_selectedPositionRow;
    QList<QSpinBox*> m_pointSpinBoxes;
    QPushButton* m_validateButton;
    QPushButton* m_cancelButton;
    QPushButton* m_closeButton;
    QPushButton* m_addNewPositionButton;
    QPushButton* m_deletePositionButton;

    QList<Position> m_originalPositions;
    QList<Position> m_newPositions;

    void populatePositionTable();
    void setPositionOnTable(const Position& position, int row);
signals:

public slots:
    void validate();
    void addNewPosition();
    void deletePosition();
    void positionClicked(int row, int column);
    void positionChanged(int row, int column);
    void cancel();
};

#endif // MANAGEPOSITIONVIEW_H

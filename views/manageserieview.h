#ifndef MANAGESERIEVIEW_H
#define MANAGESERIEVIEW_H

#include <QWidget>
#include <QDialog>
#include <QComboBox>
#include <QTableWidget>
#include <QPushButton>
#include "models/serie.h"
#include "models/position.h"

class ManageSerieView : public QDialog
{
    Q_OBJECT
public:
    explicit ManageSerieView(QWidget *parent = 0);
    ~ManageSerieView();
private:
    QComboBox* m_serieComboBox;
    QTableWidget* m_positionsTable;
    QComboBox* m_yogaPointComboBox;
    QPushButton* m_addPositionButton;
    QPushButton* m_removePositionButton;
    QPushButton* m_validateButton;
    QPushButton* m_cancelButton;
    QPushButton* m_closeButton;

    std::vector<std::shared_ptr<YogaPoint>> m_availableYogaPoint;
    QList<Serie> m_originalSeries;
    QList<Serie> m_newSeries;

    void populateYogaPoint();
    void populateSerieComboBox();
    void populatePositionTable(const Serie &serie);
    void setPositionOnTable(const Position& position, int row);
signals:

public slots:
    void serieSelected(int index);
};

#endif // MANAGESERIEVIEW_H

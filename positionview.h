#ifndef POSITIONVIEW_H
#define POSITIONVIEW_H

#include <QWidget>
#include <QtWidgets>
#include "position.h"
#include "serie.h"

class PositionView : public QWidget
{
    Q_OBJECT
public:
    explicit PositionView(QWidget *parent = 0);
    ~PositionView();
    const QPushButton *getGoBackButton();
private:
    QTableWidget *m_positionTable;
    QMap<QString, unsigned int> m_positionList;
    QList<YogaPoint*> m_positions;
    QComboBox *m_addPositionComboBox;
    QSpinBox *m_addPositionSpinBox;
    QLabel *m_timesPointLabel;
    QSignalMapper *m_signalMapper;
    QLabel *m_pointLabel;
    QLabel *m_dateLabel;
    QCalendarWidget *m_calendar;
    QPushButton *m_validateButton;
signals:

public slots:
    void addPosition();
    void positionChanged(const QString &text);
    void positionTableCellClicked(int row, int column);
    void updatePoints(int row = 0, int column = 0);
    void validateDailyPositions();
    void dateSelected(const QDate &date);
};

#endif // POSITIONVIEW_H

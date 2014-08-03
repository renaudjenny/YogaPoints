#ifndef SERIE_H
#define SERIE_H

#include "models/yogapoint.h"
#include "models/position.h"
#include <QList>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>
#include <QSqlError>

class Serie : public YogaPoint
{
public:
    Serie();
    Serie(int id, const QString &name, QList<YogaPoint*> yogaPositions);
    ~Serie();
    unsigned int calculatePoints();
    QList<YogaPoint*> getPositions();
    void setPositions(QList<YogaPoint*> positions);

    static Serie serieFromDatabase(const QString &serieName, QList<YogaPoint*> availablePositions, QWidget *window);
    static Serie serieFromDatabase(int serieId, QList<YogaPoint *> availablePositions, QWidget* window);
private:
    QList<YogaPoint*> m_positions;
};

#endif // SERIE_H

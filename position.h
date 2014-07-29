#ifndef POSITION_H
#define POSITION_H

#include "yogapoint.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>
#include <QSqlError>

class Position : public YogaPoint
{
public:
    Position();
    Position(int id, const QString& name, unsigned int points);
    ~Position();
    unsigned int calculatePoints();

    static Position positionFromDatabase(const QString &positionName, QWidget *window);
    static Position positionFromDatabase(int positionId, QWidget* window);
};

#endif // POSITION_H

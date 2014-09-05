#ifndef POSITION_H
#define POSITION_H

#include "models/yogapoint.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QVariant>
#include <QSqlError>

class Position : public YogaPoint
{
public:
    Position();
    Position(const QString& name, unsigned int points, int id = 0);
    ~Position();
    unsigned int calculatePoints() const;
    void save(QWidget* window);
    void deleteFromDB(QWidget* window);

    static Position positionFromDatabase(const QString &positionName, QWidget *window);
    static Position positionFromDatabase(int positionId, QWidget* window);
};

#endif // POSITION_H

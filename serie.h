#ifndef SERIE_H
#define SERIE_H

#include "yogapoint.h"
#include "position.h"
#include <QList>

class Serie : public YogaPoint
{
public:
    Serie();
    Serie(int id, const QString &name, unsigned int points);
    ~Serie();
    unsigned int calculatePoints();
private:
    QList<Position> m_positions;
};

#endif // SERIE_H

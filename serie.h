#ifndef SERIE_H
#define SERIE_H

#include "yogapoint.h"
#include "position.h"
#include <QList>

class Serie : public YogaPoint
{
public:
    Serie();
    Serie(int id, const QString &name, QList<YogaPoint*> yogaPositions);
    ~Serie();
    unsigned int calculatePoints();
private:
    QList<YogaPoint*> m_positions;
};

#endif // SERIE_H

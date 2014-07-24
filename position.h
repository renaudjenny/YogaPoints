#ifndef POSITION_H
#define POSITION_H

#include "yogapoint.h"

class Position : public YogaPoint
{
public:
    Position();
    Position(int id, const QString& name, unsigned int points);
    ~Position();
};

#endif // POSITION_H

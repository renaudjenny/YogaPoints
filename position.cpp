#include "position.h"

Position::Position()
{
}

Position::Position(int id, const QString &name, unsigned int points) : YogaPoint(id, name, points)
{
}

Position::~Position()
{
}

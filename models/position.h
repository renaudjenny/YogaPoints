#ifndef POSITION_H
#define POSITION_H

#include "models/yogapoint.h"

class Position : public YogaPoint
{
public:
    Position();
    Position(const std::string& name, unsigned int points, int id = 0);
    ~Position();
    unsigned int calculatePoints() const;
    void save();
    void deleteFromDB();

    static Position positionFromDatabase(const std::string &positionName);
    static Position positionFromDatabase(int positionId);

    friend bool operator==(const Position& lhs, const Position& rhs);
    friend bool operator<(const Position& lhs, const Position& rhs);
};

#endif // POSITION_H

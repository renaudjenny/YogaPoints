#include "serie.h"

Serie::Serie()
{
}

Serie::Serie(int id, const QString &name, unsigned int points) : YogaPoint(id, name, points)
{

}

Serie::~Serie()
{

}

unsigned int Serie::calculatePoints()
{
    unsigned int sum = 0;
    for (auto it = m_positions.begin(); it != m_positions.end(); it++) {
        sum += it->calculatePoints();
    }
    return sum;
}

#ifndef SERIE_H
#define SERIE_H

#include "models/yogapoint.h"
#include "models/position.h"

class Serie : public YogaPoint
{
public:
    Serie();
    Serie(const std::string &name, std::vector<std::shared_ptr<YogaPoint>> yogaPositions, int id = 0);
    ~Serie();
    unsigned int calculatePoints() const;
    std::vector<std::shared_ptr<YogaPoint>> getYogaPoints() const;
    void setYogaPoints(std::vector<std::shared_ptr<YogaPoint>> yogaPoints);
    void addYogaPoint(std::shared_ptr<YogaPoint> yogaPoint);

    static Serie serieFromDatabase(const std::string &serieName, std::vector<std::shared_ptr<YogaPoint>> availableYogaPoints);
    static Serie serieFromDatabase(int serieId, std::vector<std::shared_ptr<YogaPoint>> availableYogaPoints);
    void save();
    void deleteFromDB();
private:
    std::vector<std::shared_ptr<YogaPoint>> m_yogaPoints;
};

#endif // SERIE_H

#ifndef YOGAPOINT_H
#define YOGAPOINT_H
#include "managers/databasemanager.h"

class YogaPoint
{
public:
    YogaPoint();
    YogaPoint(int id, const std::string& name, unsigned int points = 0);
    YogaPoint(const YogaPoint& other);
    virtual ~YogaPoint();

    virtual unsigned int calculatePoints() const = 0;
    std::string name() const;
    int id() const;
    void setId(int id);
    void setPoint(int points);
    void setName(const std::string &name);
    static bool isSerie(const std::string& yogaPointName);
    virtual void save() = 0;
    virtual void deleteFromDB() = 0;
protected:
    std::string m_name;
    unsigned int m_points;
    int m_id;
};

#endif // YOGAPOINT_H

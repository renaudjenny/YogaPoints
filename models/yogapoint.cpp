#include "yogapoint.h"
#include <QSqlQuery>
#include <QSqlError>

using namespace std;

YogaPoint::YogaPoint()
{
}

YogaPoint::YogaPoint(int id, const string &name, unsigned int points) : m_name(name), m_points(points), m_id(id)
{
}

YogaPoint::YogaPoint(const YogaPoint &other)
{
    m_id = other.id();
    m_name = other.name();
    m_points = other.calculatePoints();
}

YogaPoint::~YogaPoint()
{
}

string YogaPoint::name() const
{
    return m_name;
}

int YogaPoint::id() const
{
    return m_id;
}

void YogaPoint::setId(int id)
{
    m_id = id;
}

void YogaPoint::setPoint(int points)
{
    m_points = points;
}

void YogaPoint::setName(const string &name)
{
    m_name = name;
}

bool YogaPoint::isSerie(const string &yogaPointName)
{
    QSqlQuery query("SELECT is_serie FROM yoga_point WHERE name = ?");
    query.addBindValue(QString::fromStdString(yogaPointName));
    if (!query.exec()) {
        throw runtime_error("Database error" + query.lastError().text().toStdString());
    }
    query.next();
    return query.value(0).toInt() > 0;
}

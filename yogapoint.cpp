#include "yogapoint.h"

YogaPoint::YogaPoint()
{
}

YogaPoint::YogaPoint(int id, const QString &name, unsigned int points) : m_name(name), m_points(points), m_id(id)
{
}

YogaPoint::~YogaPoint()
{
}

QString YogaPoint::name() const
{
    return m_name;
}

int YogaPoint::id()
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

void YogaPoint::setName(const QString &name)
{
    m_name = name;
}

bool YogaPoint::isSerie(const QString &positionName, QWidget* window)
{
    QSqlQuery query("SELECT COUNT(*) FROM series WHERE name = ?");
    query.addBindValue(positionName);
    if (!query.exec()) {
        QMessageBox::critical(window, "Database error", query.lastError().text());
    }
    query.next();
    return query.value(0).toInt() > 0;
}

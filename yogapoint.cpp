#include "yogapoint.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QObject>

YogaPoint::YogaPoint()
{
}

YogaPoint::YogaPoint(int id, const QString &name, unsigned int points) : m_name(name), m_points(points), m_id(id)
{
}

YogaPoint::~YogaPoint()
{
}

unsigned int YogaPoint::calculatePoints()
{
    return m_points;
}

QString YogaPoint::name() const
{
    return m_name;
}

int YogaPoint::id()
{
    return m_id;
}

int YogaPoint::setId(int id)
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

YogaPoint YogaPoint::yogaPointFromDatabase(const QString &positionName, QWidget* window)
{
    YogaPoint yogaPoint;

    QSqlQuery query("SELECT id, point FROM positions WHERE name = ?");
    query.addBindValue(positionName);
    if (!query.exec()) {
        QMessageBox::critical(window, QObject::tr("Database error"), query.lastError().text());
    }
    if (query.next()) {
        int id = query.value(0).toInt();
        int point = query.value(1).toInt();
        yogaPoint.setId(id);
        yogaPoint.setName(positionName);
        yogaPoint.setPoint(point);
    }

    return yogaPoint;
}

YogaPoint YogaPoint::yogaPointFromDatabase(int positionId, QWidget *window)
{
    YogaPoint yogaPoint;

    QSqlQuery query("SELECT name, point FROM positions WHERE id = ?");
    query.addBindValue(positionId);
    if (!query.exec()) {
        QMessageBox::critical(window, QObject::tr("Database error"), query.lastError().text());
    }
    if (query.next()) {
        QString name = query.value(0).toString();
        int point = query.value(1).toInt();
        yogaPoint.setId(positionId);
        yogaPoint.setName(name);
        yogaPoint.setPoint(point);
    }

    return yogaPoint;
}

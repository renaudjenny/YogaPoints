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

unsigned int Position::calculatePoints()
{
    return m_points;
}

Position Position::positionFromDatabase(const QString &positionName, QWidget *window)
{
    Position position;

    QSqlQuery query("SELECT id, point FROM positions WHERE name = ?");
    query.addBindValue(positionName);
    if (!query.exec()) {
        QMessageBox::critical(window, QObject::tr("Database error"), query.lastError().text());
    }
    if (query.next()) {
        int id = query.value(0).toInt();
        int point = query.value(1).toInt();
        position.setId(id);
        position.setName(positionName);
        position.setPoint(point);
    }

    return position;
}

Position Position::positionFromDatabase(int positionId, QWidget *window)
{
    Position position;

    QSqlQuery query("SELECT name, point FROM positions WHERE id = ?");
    query.addBindValue(positionId);
    if (!query.exec()) {
        QMessageBox::critical(window, QObject::tr("Database error"), query.lastError().text());
    }
    if (query.next()) {
        QString name = query.value(0).toString();
        int point = query.value(1).toInt();
        position.setId(positionId);
        position.setName(name);
        position.setPoint(point);
    }

    return position;
}

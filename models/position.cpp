#include "position.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

using namespace std;

Position::Position()
{
    m_id = 0;
}

Position::Position(const string &name, unsigned int points, int id) : YogaPoint(id, name, points)
{
}

Position::~Position()
{
}

unsigned int Position::calculatePoints() const
{
    return m_points;
}

void Position::save()
{
    //test if the serie name already exists
    QSqlQuery query("SELECT COUNT(*) FROM yoga_point WHERE id = ?");
    query.addBindValue(m_id);
    if (!query.exec()) {
        throw runtime_error("Database error" + query.lastError().text().toStdString());
    }
    query.next();
    int result = query.value(0).toInt();
    if (result == 0) {
        //Add a position
        QSqlQuery insertQuery("INSERT INTO yoga_point (name, point) VALUES (?, ?)");
        insertQuery.addBindValue(QString::fromStdString(m_name));
        insertQuery.addBindValue(m_points);
        if (!insertQuery.exec()) {
            throw runtime_error("Database error" + query.lastError().text().toStdString());
        } else {
            setId(insertQuery.lastInsertId().toInt());
        }
    } else {
        QSqlQuery updateQuery("UPDATE yoga_point SET name = ?, point = ? WHERE id = ?");
        updateQuery.addBindValue(QString::fromStdString(m_name));
        updateQuery.addBindValue(m_points);
        updateQuery.addBindValue(m_id);
        if (!updateQuery.exec()) {
            throw runtime_error("Database error" + updateQuery.lastError().text().toStdString());
        }
    }
}

void Position::deleteFromDB()
{
    QSqlQuery deleteQuery("DELETE FROM yoga_point WHERE id = ?");
    deleteQuery.addBindValue(m_id);
    if (!deleteQuery.exec()) {
        throw runtime_error("Database error" + deleteQuery.lastError().text().toStdString());
    }
}

Position Position::positionFromDatabase(const string &positionName)
{
    Position position;

    QSqlQuery query("SELECT id, point FROM yoga_point WHERE name = ?");
    query.addBindValue(QString::fromStdString(positionName));
    if (!query.exec()) {
        throw runtime_error("Database error" + query.lastError().text().toStdString());
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

Position Position::positionFromDatabase(int positionId)
{
    Position position;

    QSqlQuery query("SELECT name, point FROM yoga_point WHERE id = ?");
    query.addBindValue(positionId);
    if (!query.exec()) {
        throw runtime_error("Database error" + query.lastError().text().toStdString());
    }
    if (query.next()) {
        string name = query.value(0).toString().toStdString();
        int point = query.value(1).toInt();
        position.setId(positionId);
        position.setName(name);
        position.setPoint(point);
    }

    return position;
}

bool operator==(const Position &lhs, const Position &rhs)
{
    return lhs.name() == rhs.name();
}

bool operator<(const Position &lhs, const Position &rhs)
{
    return lhs.id() < rhs.id();
}

#include "position.h"

Position::Position()
{
}

Position::Position(const QString &name, unsigned int points, int id) : YogaPoint(id, name, points)
{
}

Position::~Position()
{
}

unsigned int Position::calculatePoints() const
{
    return m_points;
}

void Position::save(QWidget *window)
{
    //test if the serie name already exists
    QSqlQuery query("SELECT COUNT(*) FROM positions WHERE name = ?");
    query.addBindValue(m_name);
    if (!query.exec()) {
        QMessageBox::critical(window, QObject::tr("Database error"), query.lastError().text());
    }
    query.next();
    int result = query.value(0).toInt();
    if (result == 0) {
        //Add a position
        QSqlQuery insertQuery("INSERT INTO positions (name, point) VALUES (?, ?)");
        insertQuery.addBindValue(m_name);
        insertQuery.addBindValue(m_points);
        if (!insertQuery.exec()) {
            QMessageBox::critical(window, QObject::tr("Database error"), query.lastError().text());
        }
    } else {
        QSqlQuery updateQuery("UPDATE positions SET name = ?, point = ? WHERE id = ?");
        updateQuery.addBindValue(m_name);
        updateQuery.addBindValue(m_points);
        updateQuery.addBindValue(m_id);
        if (!updateQuery.exec()) {
            QMessageBox::critical(window, QObject::tr("Database error"), updateQuery.lastError().text());
        }
    }
}

void Position::deleteFromDB(QWidget *window)
{
    QSqlQuery deleteQuery("DELETE FROM positions WHERE id = ?");
    deleteQuery.addBindValue(m_id);
    if (!deleteQuery.exec()) {
        QMessageBox::critical(window, QObject::tr("Database error"), deleteQuery.lastError().text());
    }
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

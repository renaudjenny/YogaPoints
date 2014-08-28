#include "serie.h"

Serie::Serie()
{
}

Serie::Serie(const QString &name, QList<YogaPoint *> yogaPositions, int id) : YogaPoint(id, name), m_positions(yogaPositions)
{
}

Serie::~Serie()
{
}

unsigned int Serie::calculatePoints()
{
    unsigned int sum = 0;
    for (auto it = m_positions.begin(); it != m_positions.end(); it++) {
        sum += (*it)->calculatePoints();
    }
    return sum;
}

QList<YogaPoint *> Serie::getPositions()
{
    return m_positions;
}

void Serie::setPositions(QList<YogaPoint *> positions)
{
    m_positions = positions;
}

Serie Serie::serieFromDatabase(const QString &serieName, QList<YogaPoint *> availablePositions, QWidget *window)
{
    Serie serie;

    QSqlQuery query("SELECT id, position_id FROM series WHERE name = ?");
    query.addBindValue(serieName);
    if (!query.exec()) {
        QMessageBox::critical(window, QObject::tr("Database error"), query.lastError().text());
    }
    int id = -1;
    QList<YogaPoint*> positions;
    if (query.next()) {
        if (id == -1) {
            id = query.value(0).toInt();
        }
        int positionId = query.value(1).toInt();
        for (YogaPoint* yogaPoint : availablePositions) {
            if (yogaPoint->id() == positionId) {
                positions.append(yogaPoint);
            }
        }
    }
    serie.setId(id);
    serie.setName(serieName);
    serie.setPositions(positions);

    return serie;
}

Serie Serie::serieFromDatabase(int serieId, QList<YogaPoint *> availablePositions, QWidget *window)
{
    Serie serie;

    QSqlQuery query("SELECT name, position_id FROM series WHERE id = ?");
    query.addBindValue(serieId);
    if (!query.exec()) {
        QMessageBox::critical(window, QObject::tr("Database error"), query.lastError().text());
    }
    QString name = "Not found";
    QList<YogaPoint*> positions;
    if (query.next()) {
        if (name == "Not found") {
            name = query.value(0).toString();
        }
        int positionId = query.value(1).toInt();
        for (YogaPoint* yogaPoint : availablePositions) {
            if (yogaPoint->id() == positionId) {
                positions.append(yogaPoint);
            }
        }
    }
    serie.setId(serieId);
    serie.setName(name);
    serie.setPositions(positions);

    return serie;
}

void Serie::save(QWidget *window)
{
    //test if the serie name already exists
    QSqlQuery query("SELECT COUNT(*) FROM serie WHERE name = ?");
    query.addBindValue(m_name);
    if (!query.exec()) {
        QMessageBox::critical(window, QObject::tr("Database error"), query.lastError().text());
    }
    query.next();
    int result = query.value(0).toInt();
    if (result == 0) {
        //Add a serie
        for (YogaPoint* position : m_positions) {
            QSqlQuery insertQuery("INSERT INTO serie (name, id, position_id) VALUES (?, ?, ?)");
            insertQuery.addBindValue(m_name);
            insertQuery.addBindValue(nextAvailableSerieId(window));
            insertQuery.addBindValue(position->id());
            if (!insertQuery.exec()) {
                QMessageBox::critical(window, QObject::tr("Database error"), query.lastError().text());
            }
        }
    } else {
        //TODO Update the serie
        //the best way to do that for the serie is to delete and recreate the serie
    }
}

void Serie::deleteFromDB(QWidget *window)
{
    //TODO
}

int Serie::nextAvailableSerieId(QWidget *window)
{
    QSqlQuery query("SELECT MAX(id) FROM serie GROUP BY id");
    if (!query.exec()) {
        QMessageBox::critical(window, QObject::tr("Database error"), query.lastError().text());
    }
    query.next();
    int result = query.value(0).toInt();
    qDebug() << "Max id: " << result;
    return result + 1;
}

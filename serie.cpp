#include "serie.h"

Serie::Serie()
{
}

Serie::Serie(int id, const QString &name, QList<YogaPoint *> yogaPositions) : YogaPoint(id, name), m_positions(yogaPositions)
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

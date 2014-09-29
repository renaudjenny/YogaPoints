#include "serie.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

using namespace std;

Serie::Serie()
{
}

Serie::Serie(const string &name, vector<shared_ptr<YogaPoint>> yogaPositions, int id) : YogaPoint(id, name), m_yogaPoints(yogaPositions)
{
}

Serie::~Serie()
{
}

unsigned int Serie::calculatePoints() const
{
    unsigned int sum = 0;
    for_each(m_yogaPoints.begin(), m_yogaPoints.end(), [&sum](shared_ptr<YogaPoint> yogaPoint) { sum += yogaPoint->calculatePoints(); });
    /*
    for (auto it = m_yogaPoints.begin(); it != m_yogaPoints.end(); it++) {
        sum += (*it)->calculatePoints();
    }
    */
    return sum;
}

vector<shared_ptr<YogaPoint>> Serie::getYogaPoints() const
{
    return m_yogaPoints;
}

void Serie::setYogaPoints(vector<shared_ptr<YogaPoint>> yogaPoints)
{
    m_yogaPoints = yogaPoints;
}

void Serie::addYogaPoint(shared_ptr<YogaPoint> yogaPoint)
{
    m_yogaPoints.push_back(yogaPoint);
}

Serie Serie::serieFromDatabase(const string &serieName, vector<shared_ptr<YogaPoint>> availableYogaPoints)
{
    Serie serie;

    QSqlQuery query("SELECT id FROM yoga_point WHERE yoga_point.name = ? AND is_serie = 1");
    query.addBindValue(QString::fromStdString(serieName));
    if (!query.exec()) {
        throw runtime_error("Database error" + query.lastError().text().toStdString());
    }
    if (query.next()) {
        int id = query.value(0).toInt();
        QSqlQuery positionQuery("SELECT yoga_point_id FROM series WHERE id = ?");
        positionQuery.addBindValue(id);
        std::vector<shared_ptr<YogaPoint>> yogaPoints;
        while (positionQuery.next()) {
            int yogaPointId = query.value(1).toInt();
            for (shared_ptr<YogaPoint> yogaPoint : availableYogaPoints) {
                if (yogaPoint->id() == yogaPointId) {
                    yogaPoints.push_back(yogaPoint);
                }
            }
        }
        serie.setId(id);
        serie.setName(serieName);
        serie.setYogaPoints(yogaPoints);
    }

    return serie;
}

Serie Serie::serieFromDatabase(int serieId, vector<shared_ptr<YogaPoint>> availableYogaPoints)
{
    Serie serie;

    QSqlQuery query("SELECT yoga_point.id, series.yoga_point_id FROM yoga_point WHERE yoga_point.id = ? AND yoga_point.is_serie = 1 INNER JOIN series ON yoga_point.id = series.id");
    query.addBindValue(serieId);
    if (!query.exec()) {
        throw runtime_error("Database error: " + query.lastError().text().toStdString());
    }
    QString name = "Not found";
    vector<shared_ptr<YogaPoint>> positions;
    if (query.next()) {
        if (name == "Not found") {
            name = query.value(0).toString();
        }
        int positionId = query.value(1).toInt();
        for (shared_ptr<YogaPoint> yogaPoint : availableYogaPoints) {
            if (yogaPoint->id() == positionId) {
                positions.push_back(yogaPoint);
            }
        }
    }
    serie.setId(serieId);
    serie.setName(name.toStdString());
    serie.setYogaPoints(positions);

    return serie;
}

void Serie::save()
{
    //test if the serie name already exists
    QSqlQuery query("SELECT COUNT(*) FROM yoga_point WHERE name = ? AND is_serie = 1");
    query.addBindValue(QString::fromStdString(m_name));
    if (!query.exec()) {
        throw runtime_error("Database error" + query.lastError().text().toStdString());
    }
    query.next();
    int result = query.value(0).toInt();
    if (result == 0) {
        //Add a serie
        //TODO Test infinite recursion (a serie can contain another serie who contain the first one...)
        QSqlQuery insertQuery("INSERT INTO yoga_point (name, is_serie) VALUES (?, 1)");
        insertQuery.addBindValue(QString::fromStdString(m_name));
        if (!insertQuery.exec()) {
            throw runtime_error("Database error" + insertQuery.lastError().text().toStdString());
        } else {
            setId(insertQuery.lastInsertId().toInt());
            for (std::shared_ptr<YogaPoint> position : m_yogaPoints) {
                QSqlQuery insertYogaPointQuery("INSERT INTO series (id, yoga_point_id) VALUES (?, ?)");
                insertYogaPointQuery.addBindValue(m_id);
                insertYogaPointQuery.addBindValue(position->id());
                if (!insertYogaPointQuery.exec()) {
                    throw runtime_error("Database error" + insertYogaPointQuery.lastError().text().toStdString());
                }
            }
        }
    } else {
        //TODO Update the serie
    }
}

void Serie::deleteFromDB()
{
    //TODO
}

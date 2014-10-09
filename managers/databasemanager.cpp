#include "databasemanager.h"
#include "models/position.h"
#include "models/serie.h"

using namespace std;

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
}

bool DatabaseManager::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("my.db.sqlite");
    return db.open();
}

bool DatabaseManager::deleteDB()
{
    db.close();
    return QFile::remove("my.db.sqlite");
}

QSqlError DatabaseManager::lastError()
{
    return db.lastError();
}

void DatabaseManager::createDatabaseSchema()
{
    if (openDB()) {
        QSqlQuery createTableQuery;

        //first thing, we'll set yoga_point table on database
        if (!createTableQuery.prepare("CREATE TABLE IF NOT EXISTS yoga_point (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(100) UNIQUE, point INT, is_serie TINYINT)")) {
            throw runtime_error(createTableQuery.lastError().text().toStdString());
        }
        if (!createTableQuery.exec()) {
            throw runtime_error(createTableQuery.lastError().text().toStdString());
        }
        //and series table
        if (!createTableQuery.prepare("CREATE TABLE IF NOT EXISTS series (id INT, yoga_point_id INT)")) {
            throw runtime_error(createTableQuery.lastError().text().toStdString());
        }
        if (!createTableQuery.exec()) {
            throw runtime_error(createTableQuery.lastError().text().toStdString());
        }

        vector<shared_ptr<YogaPoint>> yogaPoints;

        //we read position from JSON file and copy them in Database if positions is empty
        QSqlQuery selectCountYogaPoint("SELECT COUNT(*) FROM yoga_point");
        selectCountYogaPoint.next();
        int yogaPointCount = selectCountYogaPoint.value(0).toInt();

        if (yogaPointCount == 0)
        {
            QFile positionJsonFile(":/positions.json");
            if (positionJsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString jsonString = positionJsonFile.readAll();
                positionJsonFile.close();
                QJsonDocument positionJson = QJsonDocument::fromJson(jsonString.toUtf8());
                QJsonObject positions = positionJson.object();
                for (QJsonObject::iterator it = positions.begin(); it != positions.end(); it++) {
                    string positionName = it.key().toStdString();
                    int positionPoint = it.value().toInt();
                    shared_ptr<YogaPoint> position = make_shared<Position>(Position(positionName, positionPoint));
                    yogaPoints.push_back(position);
                    position->save();
                }
            } else {
                throw runtime_error("Error while reading JSON position file");
            }
            QFile serieJsonFile(":/series.json");
            if (serieJsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString jsonString = serieJsonFile.readAll();
                serieJsonFile.close();
                QJsonDocument serieJson = QJsonDocument::fromJson(jsonString.toUtf8());
                QJsonObject series = serieJson.object();
                for (QJsonObject::iterator it = series.begin(); it != series.end(); it++) {
                    string serieName = it.key().toStdString();
                    shared_ptr<YogaPoint> serie = make_shared<Serie>(Serie(serieName, yogaPoints));
                    yogaPoints.push_back(serie);
                    QJsonArray serieYogaPointNames = it.value().toArray();
                    for (QJsonArray::iterator iit = serieYogaPointNames.begin(); iit != serieYogaPointNames.end(); iit++) {
                        //find Yoga Point matching the name
                        string yogaPointName = (*iit).toString().toStdString();
                        auto yogaPointIt = find_if(yogaPoints.begin(), yogaPoints.end(), [&yogaPointName](shared_ptr<YogaPoint> yogaPoint) { return yogaPointName == yogaPoint->name(); });
                        if (yogaPointIt != yogaPoints.end()) {
                            dynamic_pointer_cast<Serie>(serie)->addYogaPoint(*yogaPointIt);
                        }
                    }
                    serie->save();
                }
            }
        }

        //now we create a table recording daily positions
        QSqlQuery CreateTableDailyPositionsQuery;
        if (!CreateTableDailyPositionsQuery.prepare("CREATE TABLE IF NOT EXISTS daily_positions (days DATE, position_id INT, times TINYINT, is_serie BOOLEAN DEFAULT 0 NOT NULL)")) {
            throw runtime_error(CreateTableDailyPositionsQuery.lastError().text().toStdString());
        }
        if (!CreateTableDailyPositionsQuery.exec()) {
            throw runtime_error(CreateTableDailyPositionsQuery.lastError().text().toStdString());
        }
    } else {
        throw runtime_error("Problem occured with database connection");
    }
}

#include "databasemanager.h"
#include "models/position.h"

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

void DatabaseManager::createDatabaseSchema(QWidget *window)
{
    if (openDB()) {
        QSqlQuery createTableQuery;

        //first thing, we'll set position table on database
        if (!createTableQuery.prepare("CREATE TABLE IF NOT EXISTS positions (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(100) UNIQUE, point INT)")) {
            QMessageBox::critical(window, tr("Database error"), createTableQuery.lastError().text());
        }
        if (!createTableQuery.exec()) {
            QMessageBox::critical(window, tr("Database error"), createTableQuery.lastError().text());
        }

        //we read position from JSON file and copy them in Database if positions is empty
        QSqlQuery selectCountPositions("SELECT COUNT(*) FROM positions");
        selectCountPositions.next();
        int positionCount = selectCountPositions.value(0).toInt();

        if (positionCount == 0)
        {
            QFile positionJsonFile(":/positions.json");
            if (positionJsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString jsonString = positionJsonFile.readAll();
                positionJsonFile.close();
                QJsonDocument positionJson = QJsonDocument::fromJson(jsonString.toUtf8());
                QJsonObject positions = positionJson.object();
                for (QJsonObject::iterator it = positions.begin(); it != positions.end(); it++) {
                    QString positionName = it.key();
                    int positionPoint = it.value().toInt();
                    QSqlQuery insertPositionQuery;
                    insertPositionQuery.prepare("INSERT INTO positions (name, point) VALUES (:name, :point)");
                    insertPositionQuery.bindValue(":name", positionName);
                    insertPositionQuery.bindValue(":point", positionPoint);
                    insertPositionQuery.exec();
                }
            } else {
                QMessageBox::warning(window, tr("Error while reading JSON position file"),
                    tr("Couldn't open JSON position file"));
            }
        }

        //we'll set serie table on database
        if (!createTableQuery.prepare("CREATE TABLE IF NOT EXISTS series (id INT, name VARCHAR(100) UNIQUE, position_id INT)")) {
            QMessageBox::critical(window, tr("Database error"), createTableQuery.lastError().text());
        }
        if (!createTableQuery.exec()) {
            QMessageBox::critical(window, tr("Database error"), createTableQuery.lastError().text());
        }

        //we read position from JSON file and copy them in Database if positions is empty
        QSqlQuery selectCountSeries("SELECT COUNT(*) FROM series");
        selectCountSeries.next();
        int serieCount = selectCountSeries.value(0).toInt();

        if (serieCount == 0)
        {
            QFile serieJsonFile(":/series.json");
            if (serieJsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString jsonString = serieJsonFile.readAll();
                serieJsonFile.close();
                QJsonDocument serieJson = QJsonDocument::fromJson(jsonString.toUtf8());
                QJsonObject series = serieJson.object();
                int i = 0;
                for (QJsonObject::iterator it = series.begin(); it != series.end(); it++) {
                    QString serieName = it.key();
                    QJsonArray seriePositionNames = it.value().toArray();
                    for (QJsonArray::iterator iit = seriePositionNames.begin(); iit != seriePositionNames.end(); iit++) {
                        Position position = Position::positionFromDatabase((*iit).toString(), window);
                        QSqlQuery insertSerieQuery;
                        insertSerieQuery.prepare("INSERT INTO series (id, name, position_id) VALUES (?, ?, ?)");
                        insertSerieQuery.addBindValue(i);
                        insertSerieQuery.addBindValue(serieName);
                        insertSerieQuery.addBindValue(position.id());
                        insertSerieQuery.exec();
                    }
                    i++;
                }
            } else {
                QMessageBox::warning(window, tr("Error while reading JSON position file"),
                    tr("Couldn't open JSON position file"));
            }
        }

        //now we create a table recording daily positions
        QSqlQuery CreateTableDailyPositionsQuery;
        if (!CreateTableDailyPositionsQuery.prepare("CREATE TABLE IF NOT EXISTS daily_positions (days DATE, position_id INT, times TINYINT, is_serie BOOLEAN DEFAULT 0 NOT NULL)")) {
            QMessageBox::critical(window, tr("Database error"), CreateTableDailyPositionsQuery.lastError().text());
        }
        if (!CreateTableDailyPositionsQuery.exec()) {
            QMessageBox::critical(window, tr("Database error"), CreateTableDailyPositionsQuery.lastError().text());
        }
    } else {
        QMessageBox::critical(window, tr("Database issue"), tr("Problem occured with database connection"));
    }
}

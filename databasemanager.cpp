#include "databasemanager.h"

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

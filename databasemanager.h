#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql>
#include <QFile>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);
    bool openDB();
    bool deleteDB();
    QSqlError lastError();

private:
        QSqlDatabase db;

signals:

public slots:

};

#endif // DATABASEMANAGER_H

#include "mainwindow.h"
#include "databasemanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    DatabaseManager databaseManager;
    if (databaseManager.openDB()) {
        QSqlQuery createTableQuery;

        //first thing, we'll set position table on database
        if (!createTableQuery.prepare("CREATE TABLE IF NOT EXISTS positions (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR(100), point INT)")) {
            QMessageBox::critical(this, tr("Database error"), createTableQuery.lastError().text());
        }
        if (!createTableQuery.exec()) {
            QMessageBox::critical(this, tr("Database error"), createTableQuery.lastError().text());
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
                QMessageBox::warning(this, tr("Error while reading JSON position file"),
                    tr("Couldn't open JSON position file"));
            }
        }

        //we'll set serie table on database
        if (!createTableQuery.prepare("CREATE TABLE IF NOT EXISTS series (id INT, name VARCHAR(100), position_id INT)")) {
            QMessageBox::critical(this, tr("Database error"), createTableQuery.lastError().text());
        }
        if (!createTableQuery.exec()) {
            QMessageBox::critical(this, tr("Database error"), createTableQuery.lastError().text());
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
                        Position position = Position::positionFromDatabase((*iit).toString(), this);
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
                QMessageBox::warning(this, tr("Error while reading JSON position file"),
                    tr("Couldn't open JSON position file"));
            }
        }

        //now we create a table recording daily positions
        QSqlQuery CreateTableDailyPositionsQuery;
        if (!CreateTableDailyPositionsQuery.prepare("CREATE TABLE IF NOT EXISTS daily_positions (days DATE, position_id INT, times TINYINT, is_serie BOOLEAN DEFAULT 0 NOT NULL)")) {
            QMessageBox::critical(this, tr("Database error"), CreateTableDailyPositionsQuery.lastError().text());
        }
        if (!CreateTableDailyPositionsQuery.exec()) {
            QMessageBox::critical(this, tr("Database error"), CreateTableDailyPositionsQuery.lastError().text());
        }
    } else {
        QMessageBox::critical(this, tr("Database issue"), tr("Problem occured with database connection"));
    }

    m_positionView = new PositionView(this);
    setCentralWidget(m_positionView);

    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{
    DatabaseManager db;
    db.deleteDB();
}

void MainWindow::createActions()
{
    addPositionAct = new QAction(tr("&Add a position"), this);
    addPositionAct->setShortcut(QKeySequence::New);
    addPositionAct->setToolTip(tr("Add a new position in the list"));
    connect(addPositionAct, SIGNAL(triggered()), this, SLOT(openAddPositionView()));
}

void MainWindow::createMenus()
{
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(addPositionAct);
}

void MainWindow::openAddPositionView()
{
    //TODO Create an add position view
    AddPositionView* addPositionView = new AddPositionView;
    addPositionView->show();
}

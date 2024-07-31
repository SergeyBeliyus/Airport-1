#ifndef DB_H
#define DB_H

#include <QObject>
#include <QSqlDataBase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

class DB : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase *dataBase;
    bool ConnectionStatus;
    QSqlQueryModel* modelQueryMain;
    QSqlQueryModel* modelQueryStatistics;
    QString convertInputDate(const QString& date);

public:

    enum ConnFields{
        host = 0,
        db_name = 1,
        login = 2,
        password = 3,
        port = 4
    };

    DB(QObject *parent = nullptr);
    ~DB();
    void addDB(QString driver, QString nameDB = "");
    void getArrivals(const QString& airportCode, const QString& date);
    void getDepartures(const QString& airportCode, const QString& date);
    void getDataPerYear(const QString &airportCode);
    void getDataPerMonth(const QString &airportCode);
    bool getConnectionState();
    void connectToDB();
    void disconnectFromDB(QString nameDb = "");
    QSqlError getLastError();

signals:
    void sig_SendQueryFromDB(QSqlQueryModel* model);
    void sig_SendDataToArrivals(QSqlQueryModel* model);
    void sig_SendDataToDepartures(QSqlQueryModel* model);
    void sig_SendStatusConnection(bool statusConnection);
    void sig_SendListAirports(QSqlQueryModel* model);
    void sig_SendDataPerYear(QSqlQueryModel* model);
    void sig_SendDataPerMonth(QSqlQueryModel* model);
};

#endif

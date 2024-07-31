#include "db.h"

DB::DB(QObject *parent) : QObject{parent}
{
    dataBase = new QSqlDatabase();
    modelQueryMain= new QSqlQueryModel(this);
    modelQueryStatistics = new QSqlQueryModel(this);
    ConnectionStatus = false;
}

DB::~DB()
{
    delete dataBase;
}

void DB::addDB(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
}

bool DB::getConnectionState()
{
    return ConnectionStatus;
}

void DB::connectToDB()
{
    QVector<QString> data;
    data.resize(port + 1);

    data[host] = "981757-ca08998.tmweb.ru";
    data[port] = "5432";
    data[db_name] = "demo";
    data[login] = "netology_usr_cpp";
    data[password] = "CppNeto3";

    dataBase->setHostName(data[host]);
    dataBase->setPort(data[port].toInt());
    dataBase->setDatabaseName(data[db_name]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[password]);

    ConnectionStatus = dataBase->open();

    QString request = "SELECT airport_name->>'ru' AS name, airport_code FROM bookings.airports_data ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*dataBase);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryMain->setQuery(*query);

    emit sig_SendQueryFromDB(modelQueryMain);
    emit sig_SendStatusConnection(ConnectionStatus);
    emit sig_SendListAirports(modelQueryMain);
    delete query;
}

void DB::disconnectFromDB(QString nameDb)
{
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();
}

QSqlError DB::getLastError()
{
    return dataBase->lastError();
}

void DB::getArrivals(const QString& airportCode, const QString& date)
{
    QString parsedDate = convertInputDate(date);
    QString request = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' AS name "
                      "FROM bookings.flights f "
                      "JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                      "WHERE (f.arrival_airport  = '" + airportCode + "' AND f.scheduled_arrival::date = date('" + parsedDate + "')) "
                      "ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*dataBase);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryMain->setQuery(*query);
    emit sig_SendDataToArrivals(modelQueryMain);
    delete query;
}

void DB::getDepartures(const QString &airportCode, const QString& date)
{
    QString parsedDate = convertInputDate(date);
    QString request = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' AS name "
                      "FROM bookings.flights f "
                      "JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                      "WHERE (f.departure_airport  = '" + airportCode + "' AND f.scheduled_departure::date = date('" + parsedDate + "')) "
                      "ORDER BY name";
    QSqlQuery* query = new QSqlQuery(*dataBase);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryMain->setQuery(*query);
    emit sig_SendDataToDepartures(modelQueryMain);
    delete query;
}

void DB::getDataPerYear(const QString &airportCode)
{
    QString request = "SELECT count(flight_no), date_trunc('month', scheduled_departure) AS Month "
                      "FROM bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') "
                      "AND scheduled_departure::date <= date('2017-08-31')) AND "
                      "(departure_airport = '" + airportCode + "' or arrival_airport = '" + airportCode + "') "
                      "GROUP BY Month";

    QSqlQuery* query = new QSqlQuery(*dataBase);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryStatistics->setQuery(*query);
    emit sig_SendDataPerYear(modelQueryStatistics);
    delete query;
}

void DB::getDataPerMonth(const QString &airportCode)
{
    QString request = "SELECT count(flight_no), date_part('month', date_trunc('day', scheduled_departure)), date_trunc('day', scheduled_departure) AS Day "
                      "FROM bookings.flights f "
                      "WHERE (scheduled_departure::date > date('2016-08-31') "
                      "AND scheduled_departure::date <= date('2017-08-31')) AND "
                      "(departure_airport = '" + airportCode + "' or arrival_airport = '" + airportCode + "') "
                      "GROUP BY Day";

    QSqlQuery* query = new QSqlQuery(*dataBase);
    QSqlError error;
    if(!query->exec(request)){
        error = query->lastError();
    }
    modelQueryStatistics->setQuery(*query);
    emit sig_SendDataPerMonth(modelQueryStatistics);
    delete query;
}

QString DB::convertInputDate(const QString &date)
{
    QString day, month, year;
    for(int i = 0; i < date.size(); ++i)
    {
        if(i < 2)
        {
            month+=date[i];

        }
        if(i > 2 && i < 5)
        {
            day+=date[i];
        }
        if(i > 5)
        {
            year+=date[i];
        }
    }
    QString resultStr = year + "-" + month + "-" + day;
    return resultStr;
}

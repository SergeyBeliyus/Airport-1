#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->status->setText("Не подключен к базе данных");
    ui->status->setStyleSheet("color:red");
    ui->groupBox->setEnabled(false);
    ui->statusbar->addWidget(ui->status);
    ui->rb_arrival->setChecked(true);
    ui->centralwidget->setWindowTitle("Расписание");

    dataBase = new DB(this);
    dataBase->addDB(POSTGRE_DRIVER, DB_NAME);

    statistics = new Stat();

    MessageBox = new QMessageBox(this);

    DBConnectionFail = false;

    sw = new Watchdog(this);
    sw->Start();

    connect(sw->getQTimer(), &QTimer::timeout, this, &MainWindow::ConnectToDB);
    connect(dataBase, &DB::sig_SendStatusConnection, this, &MainWindow::RcvSignalConnectToDB);
    connect(dataBase, &DB::sig_SendListAirports, this, &MainWindow::RcvSignalGetAirportList);
    connect(dataBase, &DB::sig_SendQueryFromDB, this, &MainWindow::RcvSignalQueryDB);
    connect(dataBase, &DB::sig_SendDataToArrivals, this, &MainWindow::RcvSignalQueryDB);
    connect(dataBase, &DB::sig_SendDataToDepartures, this, &MainWindow::RcvSignalQueryDB);
    connect(dataBase, &DB::sig_SendDataPerYear, this, &MainWindow::RcvSignalDTPerYear);
    connect(dataBase, &DB::sig_SendDataPerMonth, this, &MainWindow::RcvSignalDTPerMonth);
    connect(statistics, &Stat::sig_CloseStatistics, this, &MainWindow::RcvSignalCloseStatistics);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ConnectToDB()
{
    if (!dataBase->getConnectionState() && !DBConnectionFail)
    {
        auto conn = [&]{dataBase->connectToDB();};
        QtConcurrent::run(conn);

    }
    if (DBConnectionFail)
    {
        sw->setTime(sw->getCurrentTime() + 1);
        if(sw->getCurrentTime() >= 5)
        {
            DBConnectionFail = !DBConnectionFail;
        }
    }
}

void MainWindow::RcvSignalConnectToDB(bool status)
{
    if (status)
    {
        ui->status->setText("Подключено к БД");
        ui->status->setStyleSheet("color:green");
        ui->statusbar->addWidget(ui->status);
        ui->groupBox->setEnabled(true);
        sw->Stop();
    }
    else
    {
        DBConnectionFail = DBConnectionFail;
        ui->status->setText("Отключено от БД");
        ui->status->setStyleSheet("color:red");
        ui->statusbar->addWidget(ui->status);
        dataBase->disconnectFromDB(DB_NAME);
        MessageBox->setIcon(QMessageBox::Critical);
        MessageBox->setText(dataBase->getLastError().text());
        sw->Stop();
        MessageBox->exec();
        sw->setTime(0);
        sw->Start();
    }
}

void MainWindow::RcvSignalGetAirportList(QSqlQueryModel *model)
{
    for(int i = 0; i < model->rowCount(); ++i)
    {
        ui->cb_airports->addItem(model->data(model->index(i,0)).toString());
        Airports[model->data(model->index(i,0)).toString()] = model->data(model->index(i,1)).toString();
    }

}

void MainWindow::on_btn_getList_clicked()
{
    QString airportCode = Airports[ui->cb_airports->currentText()];
    QString date = ui->de_date->text();
    if(ui->rb_arrival->isChecked())
    {
        dataBase->getArrivals(airportCode, date);
    }
    else if(ui->rb_departure->isChecked())
    {
        dataBase->getDepartures(airportCode, date);
    }
}

void MainWindow::RcvSignalQueryDB(QSqlQueryModel *model)
{
   ui->tableView->setModel(model);
}

void MainWindow::on_btn_showLoad_clicked()
{
    ui->centralwidget->setEnabled(false);
    QString airportCode = ui->cb_airports->currentText() +
                          " (" + Airports[ui->cb_airports->currentText()] + ")";
    statistics->setAirportText(airportCode);
    dataBase->getDataPerYear(Airports[ui->cb_airports->currentText()]);
    dataBase->getDataPerMonth(Airports[ui->cb_airports->currentText()]);
    statistics->show();
}

void MainWindow::RcvSignalDTPerYear(QSqlQueryModel *model)
{
    QVector<double> data;
    for(int i = 0; i < model->rowCount(); ++i)
    {
        data.append(model->data(model->index(i,0)).toDouble());
    }
    statistics->setDataPerYear(data);
}

void MainWindow::RcvSignalDTPerMonth(QSqlQueryModel *model)
{
    QMultiMap<int, double> data;
    for(int i = 0; i < model->rowCount(); ++i)
    {
        data.insert(model->data(model->index(i,1)).toInt(), model->data(model->index(i,0)).toDouble());
    }
    statistics->setDataPerMonth(data);
}

void MainWindow::RcvSignalCloseStatistics()
{
    ui->centralwidget->setEnabled(true);
    statistics->close();
}

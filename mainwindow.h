#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include <QMap>

#include "db.h"
#include "stat.h"
#include "watchdog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DB* dataBase;
    Stat *statistics;
    bool DBConnectionFail;
    Watchdog *sw;
    QMessageBox *MessageBox;
    QMap<QString, QString> Airports;

private slots:
    void ConnectToDB();
    void on_btn_getList_clicked();
    void on_btn_showLoad_clicked();
    void RcvSignalConnectToDB(bool status);
    void RcvSignalGetAirportList(QSqlQueryModel *model);
    void RcvSignalQueryDB(QSqlQueryModel *model);
    void RcvSignalDTPerYear(QSqlQueryModel *model);
    void RcvSignalDTPerMonth(QSqlQueryModel *model);
    void RcvSignalCloseStatistics();
};
#endif

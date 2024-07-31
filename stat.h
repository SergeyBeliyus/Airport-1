#ifndef STAT_H
#define STAT_H

#include <QWidget>
#include <QCloseEvent>
#include "db.h"
#include "qcustomplot.h"

namespace Ui {
class Stat;
}

class Stat: public QWidget
{
    Q_OBJECT

private:
    Ui::Stat *ui; //<! Главное окно статистики загруженности аэропорта.
    QMap<int, QString> months; //<! Месяцы.
    QCPBars *CPBarsForYear; //<! Гистограмма статистики за год.
    QMultiMap<int, double> dataMonth; //<! Данные за месяцы.
    bool isDataMonthReady; //<! Готовность данных о статистике за месяц.

public:

    explicit Stat(QWidget *parent = nullptr);
    ~Stat();
    void setAirportText(const QString& airport);
    void setDataPerYear(const QVector<double> data);
    void setDataPerMonth(const QMultiMap<int, double> data);
    void setStatisticsPerMonth(const int month);
    void closeEvent(QCloseEvent* event) override;
    int findMonth(const QString& month);

private slots:

    void on_btn_close_clicked();
    void on_months_currentTextChanged(const QString &arg1);

signals:

    void sig_CloseStatistics();

};

#endif

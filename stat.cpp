#include "stat.h"
#include "ui_stat.h"

Stat::Stat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Stat)
{
    ui->setupUi(this);
    isDataMonthReady = false;
    ui->tabWidget->setCurrentWidget(ui->tb_year);
    months = { {1, "Январь"}, {2, "Февраль"}, {3, "Март"},
                {4, "Апрель"}, {5, "Май"}, {6, "Июнь"},
                {7, "Июль"}, {8, "Август"}, {9, "Сентябрь"},
                {10, "Октябрь"}, {11, "Ноябрь"}, {12, "Декабрь"}
              };

    for(const auto& [key, value] : months.asKeyValueRange())
    {
        ui->months->addItem(value);
    }

    CPBarsForYear = new QCPBars(ui->loadPerYear->xAxis,
                                 ui->loadPerYear->yAxis
                                 );
}

Stat::~Stat()
{
    delete CPBarsForYear;
    delete ui;
}

void Stat::setAirportText(const QString &airport)
{
    ui->airportsLoad->setText("Загруженность аэропорта " + airport);
}

void Stat::on_months_currentTextChanged(const QString &arg1)
{
    if (isDataMonthReady)
    {
        setStatisticsPerMonth(findMonth(arg1));
    }
}


void Stat::setDataPerYear(const QVector<double> data)
{
    ui->loadPerYear->clearGraphs();

    QVector<double> ticks;
    QVector<QString> labels;
    for(const auto& [key, value] : months.asKeyValueRange())
    {
        ticks << key;
        labels << value;
    }
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->loadPerYear->xAxis->setTicker(textTicker);
    ui->loadPerYear->xAxis->setTickLabelRotation(60);
    ui->loadPerYear->xAxis->setSubTicks(false);
    ui->loadPerYear->xAxis->setTickLength(0, 4);
    ui->loadPerYear->xAxis->setLabel("Месяцы");
    ui->loadPerYear->xAxis->setRange(0, months.size() + 1);
    ui->loadPerYear->xAxis->grid()->setVisible(true);

    ui->loadPerYear->yAxis->setRange(0, *std::max_element(data.begin(), data.end()) + 1);
    ui->loadPerYear->yAxis->setPadding(5);
    ui->loadPerYear->yAxis->setLabel("Загруженность аэропорта за год");
    ui->loadPerYear->yAxis->grid()->setSubGridVisible(true);
    ui->loadPerYear->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->loadPerYear->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    CPBarsForYear->setData(ticks, data);
    ui->loadPerYear->replot();
}

void Stat::setDataPerMonth(const QMultiMap<int, double> data)
{
    dataMonth = data;
    isDataMonthReady = !isDataMonthReady;
    setStatisticsPerMonth(1);
}

void Stat::setStatisticsPerMonth(const int month)
{

    ui->loadPerMonth->clearGraphs();
    ui->loadPerMonth->addGraph();
    ui->loadPerMonth->graph(0)->setPen(QPen(Qt::blue));
    QVector<double> x, y;
    int it = 0;
    for (const auto& [key, value] : dataMonth.asKeyValueRange())
    {
        if (key == month)
        {
            ++it;
            y.append(value);
            x.append(it);
        }
    }
    ui->loadPerMonth->graph(0)->setData(x, y);
    ui->loadPerMonth->graph(0)->rescaleAxes();

    ui->loadPerMonth->replot();
}

int Stat::findMonth(const QString &arg1)
{
    for (const auto& [key, value] : months.asKeyValueRange())
    {
        if (arg1 == value)
        {
            return key;
        }
    }
    return -1;
}

void Stat::on_btn_close_clicked()
{
    isDataMonthReady = !isDataMonthReady;
    ui->months->setCurrentText(months[1]);
    emit sig_CloseStatistics();
}

void Stat::closeEvent(QCloseEvent *event)
{
    isDataMonthReady = !isDataMonthReady;
    ui->months->setCurrentText(months[1]);
    emit sig_CloseStatistics();
}


#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <QObject>
#include <QTimer>
#include <QTime>

class Watchdog : public QObject
{
    Q_OBJECT
public:
    explicit Watchdog(QObject *parent = nullptr);
    // Запустить секундомер
    void Start();
    // Остановить секундомер
    void Stop();
    // Очистить секундомер
    void Clear();
    // Количество кругов
    void Lap();
    // Установить время
    void setTime(float time);
    // Установить круг
    void setLap(int lap);
    // Получить текущее время
    float getCurrentTime();
    // Получить начальное время
    float getStartTime();
    // Получить текущий круг
    int getCurrentLap();
    // Проверить, запущен ли секундомер
    bool isStart();
    //Получить адрес qTimer
    QTimer* getQTimer();

signals:
    void sig_Start();
    void sig_Stop();
    void sig_Clear();
    void sig_Lap();

private:
    bool isStart_; // флаг работы секундомера
    float currentTime_; // текущее время
    float startTime_; // время от начала работы круга
    int currentLap_; // текущий круг
    QTimer* qTimer;
};

#endif // WATCHDOG_H

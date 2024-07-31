#include "watchdog.h"

Watchdog::Watchdog(QObject *parent)
    : QObject{parent}
{
    isStart_ = false;
    currentTime_ = 0.;
    startTime_ = 0.;
    qTimer = new QTimer(this);
}

void Watchdog::Start()
{
    isStart_ = true;
    qTimer->start(5000);
}

void Watchdog::Stop()
{
    isStart_ = false;
    qTimer->stop();
}

void Watchdog::Clear()
{
    currentTime_ = 0.;
    startTime_ = 0.;
    currentLap_ = 1;
    emit sig_Clear();
}

void Watchdog::setTime(float time)
{
    currentTime_ = time;
}

float Watchdog::getCurrentTime()
{
    return currentTime_;
}

bool Watchdog::isStart()
{
    return isStart_;
}

QTimer *Watchdog::getQTimer()
{
    return qTimer;
}


#include "timer.h"

Timer::Timer()
{}

Timer::~Timer()
{}

void Timer::pause()
{
    if(isActive())stop();
    else start(interval());
}

/*#include "timer.h"

Timer::Timer(QLCDNumber *lcdNumber)
{
    lcd = lcdNumber;
}

void Timer::pause()
{
    if(paused)timer->start(1000);
    else timer->stop();
    paused = !paused;
}

void Timer::run()
{
    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(updateLCD()));
    timer->start(1000);
    exec();
}

void Timer::updateLCD()
{
    seconds++;
    lcd->display(seconds);
}*/


#ifndef TIMER_H
#define TIMER_H

#include <QTimer>

class Timer : public QTimer
{
    Q_OBJECT
public:
    Timer();
    ~Timer();

private slots:
    void pause();
};

#endif
/*#ifndef TIMER_H
#define TIMER_H

#include <QThread>
*#include <QTimer>
#include <QLCDNumber>

class Timer : public QThread
{
    Q_OBJECT

public:
    Timer(QLCDNumber *lcdNumber);

private:
    int seconds = 0;
    QLCDNumber *lcd = nullptr;
    QTimer *timer = nullptr;
    bool paused = false;

private slots:
    void pause();

protected:
    void run();

private slots:
    void updateLCD();
};

#endif // TIMER_H
*/

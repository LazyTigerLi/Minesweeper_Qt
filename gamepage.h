#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include "minebutton.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSize>
#include <QThread>
#include <QLCDNumber>
#include "timer.h"

class GamePage : public QWidget
{
    Q_OBJECT
public:
    GamePage(QWidget *parent = 0);
    ~GamePage();
    void initialize(int difficulty);

private:
    int width,height,numOfMines;
    int **mineDisrtibution = nullptr;       // -1:mine  other numbers:num of mines
    int **state = nullptr;          //0:not clicked     1:num   2:?     3:flag
    MineButton **mines = nullptr;
    QPushButton *pauseButton = nullptr;
    QPushButton *quitButton = nullptr;
    QLCDNumber *lcd = nullptr;
    int millseconds = 0;
    bool start = false;
    bool paused = false;
    QVBoxLayout *mainLayout = nullptr;
    QHBoxLayout *upperLayout = nullptr;
    QGridLayout *lowerLayout = nullptr;
    Timer *timer = nullptr;
    QThread *timerThread = nullptr;
    int minesLeft;
    int numOfClicked = 0;

    void generate(int x, int y);
    int getNumOfMines(int x,int y);
    void spread(int x,int y);
    void showAll();
    void detectVictory();
    void createTimer();

private slots:
    void pause();
    void updateLCD();
    void quit();
    void left();
    void right();
    void both();
};


#endif // GAMEPAGE_H

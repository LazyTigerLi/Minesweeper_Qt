#include "gamepage.h"
#include "minesweeper.h"
#include <QRandomGenerator>
#include <QMessageBox>

GamePage::GamePage(QWidget *parent)
    : QWidget(parent)
{
    pauseButton = new QPushButton("暂停",this);
    quitButton = new QPushButton("放弃",this);
    pauseButton->setDisabled(true);
    lcd = new QLCDNumber(2,this);

    connect(pauseButton,SIGNAL(clicked(bool)),this,SLOT(pause()));
    connect(quitButton,SIGNAL(clicked(bool)),this,SLOT(quit()));
    mainLayout = new QVBoxLayout;
    upperLayout = new QHBoxLayout;
    lowerLayout = new QGridLayout;
    upperLayout->addWidget(pauseButton);
    upperLayout->addWidget(quitButton);
    upperLayout->addWidget(lcd);
    lowerLayout->setSpacing(0);

    mainLayout->addLayout(upperLayout);
    mainLayout->addLayout(lowerLayout);
    setLayout(mainLayout);
}

GamePage::~GamePage()
{}

void GamePage::pause()
{
    if(paused)
    {
        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++)
            {
                if(state[i][j] == 1)
                {
                    mines[i][j].setStyleSheet("background-color:rgb(190,190,190);");
                    if(mineDisrtibution[i][j] != 0)mines[i][j].setText(QString::number(mineDisrtibution[i][j]));
                }
                else mines[i][j].setEnabled(true);
            }
        pauseButton->setText(tr("暂停"));
    }
    else
    {
        for(int i = 0; i < height; i++)
            for(int j = 0; j < width; j++)
            {
                if(state[i][j] == 1)
                {
                    mines[i][j].setText("");
                    mines[i][j].setStyleSheet("background-color:rgb(118, 118, 118);");
                }
                else mines[i][j].setDisabled(true);
            }
        pauseButton->setText(tr("继续"));
    }
    paused = !paused;
}

void GamePage::initialize(int difficulty)
{
    if(difficulty == 0)
    {
        width = 9;
        height = 9;
        numOfMines = 10;
    }
    else if(difficulty == 1)
    {
        width = 16;
        height = 16;
        numOfMines = 40;
    }
    else
    {
        width = 30;
        height = 16;
        numOfMines = 99;
    }
    minesLeft = numOfMines;
    numOfClicked = 0;
    mines = new MineButton *[height];
    mineDisrtibution = new int *[height];
    state = new int *[height];
    for(int i = 0; i < height; i++)
    {
        mines[i] = new MineButton[width];
        state[i] = new int[width];
        mineDisrtibution[i] = new int[width];
    }
    setStyleSheet("MineButton{background-color:rgb(118, 118, 118);}");
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
        {
            lowerLayout->addWidget(&mines[i][j],i,j);
            connect(&mines[i][j],SIGNAL(leftClick()),this,SLOT(left()));
            connect(&mines[i][j],SIGNAL(rightClick()),this,SLOT(right()));
            connect(&mines[i][j],SIGNAL(bothClick()),this,SLOT(both()));
            mines[i][j].setParent(this);
            mines[i][j].setFixedSize(30,30);
            state[i][j] = 0;
            mineDisrtibution[i][j] = -2;
        }
    show();
}

void GamePage::quit()
{
    Minesweeper *p = (Minesweeper *)parentWidget();
    p->mainLayout->removeWidget(this);
    p->resize(p->initialSize);
    for(int i = 0; i < height; i++)
    {
        delete[] mines[i];
        delete[] mineDisrtibution[i];
        delete[] state[i];
    }
    delete mines;
    delete mineDisrtibution;
    delete state;
    if(timerThread != nullptr)
    {
        timerThread->quit();
        timerThread = nullptr;
    }
    millseconds = 0;
    lcd->display(0);
    mineDisrtibution = nullptr;
    mines = nullptr;
    state = nullptr;
    start = false;
    hide();
}

void GamePage::generate(int x,int y)
{
    int minesToGenerate = numOfMines;
    while(minesToGenerate > 0)
    {
        int temp = QRandomGenerator::global()->bounded(height * width);
        int xPos = temp / width, yPos = temp % width;
        if((xPos == x && yPos == y) || mineDisrtibution[xPos][yPos] == -1)
            continue;
        mineDisrtibution[xPos][yPos] = -1;
        minesToGenerate--;
    }
    state[x][y] = 1;
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
        {
            if(mineDisrtibution[i][j] != -1)
                mineDisrtibution[i][j] = getNumOfMines(i,j);
        }
}

int GamePage::getNumOfMines(int x, int y)
{
    auto f = [](int a){return (a == -1) ? 1 : 0;};
    int sum = 0;
    for(int i = (x - 1 >= 0 ? x - 1 : 0); i < (x + 2 <= height ? x + 2 : height); i++)
        for(int j = (y - 1 >= 0 ? y - 1 : 0); j < (y + 2 <= width ? y + 2 : width); j++)
            sum += f(mineDisrtibution[i][j]);
    return sum;
}

void GamePage::left()
{
    bool find = false;
    int x,y;
    MineButton *btn = (MineButton *)sender();
    for(x = 0; x < height; x++)
    {
        for(y = 0; y < width; y++)
            if(btn == &mines[x][y])
            {
                find = true;
                break;
            }
        if(find)break;
    }
    if(start == false)
    {
        start = true;
        createTimer();
        generate(x,y);
        pauseButton->setEnabled(true);
    }
    int num = mineDisrtibution[x][y];
    if(num == -1)
    {
        QMessageBox::information(this, "Game Over!", "You are dead!",QMessageBox::Ok);
        timerThread->quit();
        timerThread = nullptr;
        showAll();
    }
    else if(num == 0)spread(x,y);
    else
    {
        numOfClicked++;
        state[x][y] = 1;
        mines[x][y].setDisabled(true);
        mines[x][y].setText(QString::number(num));
        mines[x][y].setStyleSheet("background-color:rgb(190,190,190);");
    }
    detectVictory();
}

void GamePage::right()
{
    bool find = false;
    int x,y;
    MineButton *btn = (MineButton *)sender();
    for(x = 0; x < height; x++)
    {
        for(y = 0; y < width; y++)
            if(btn == &mines[x][y])
            {
                find = true;
                break;
            }
        if(find)break;
    }
    if(start == false)
    {
        start = true;
        createTimer();
        generate(x,y);
        pauseButton->setEnabled(true);
    }
    if(state[x][y] == 0)
    {
        state[x][y] = 3;
        mines[x][y].setText("F");
        //mines[x][y].setStyleSheet("background-image:url(qrc:/mine.png);");
    }
    else if(state[x][y] == 3)
    {
        state[x][y] = 2;
        //mines[x][y].setStyleSheet("background-image:url();");
        mines[x][y].setText("?");
    }
    else if(state[x][y] == 2)
    {
        state[x][y] = 0;
        mines[x][y].setText("");
    }
}

void GamePage::both()
{
    bool find = false;
    int x,y;
    MineButton *btn = (MineButton *)sender();
    for(x = 0; x < height; x++)
    {
        for(y = 0; y < width; y++)
            if(btn == &mines[x][y])
            {
                find = true;
                break;
            }
        if(find)break;
    }
    int numOfFlags = 0;
    for(int i = (x - 1 >= 0 ? x - 1 : 0); i < (x + 2 <= height ? x + 2 : height); i++)
        for(int j = (y - 1 >= 0 ? y - 1 : 0); j < (y + 2 <= width ? y + 2 : width); j++)
            if(state[i][j] == 3)numOfFlags++;
    if(numOfFlags == mineDisrtibution[x][y])
    {
        for(int i = (x - 1 >= 0 ? x - 1 : 0); i < (x + 2 <= height ? x + 2 : height); i++)
            for(int j = (y - 1 >= 0 ? y - 1 : 0); j < (y + 2 <= width ? y + 2 : width); j++)
                if(state[i][j] == 0)
                {
                    if(mineDisrtibution[i][j] == -1)
                    {
                        QMessageBox::information(this, "Game Over!", "You are dead!",QMessageBox::Ok);
                        timerThread->quit();
                        timerThread = nullptr;
                        showAll();
                        return;
                    }
                    mines[i][j].setStyleSheet("background-color:rgb(190,190,190);");
                    state[i][j] = 1;
                    if(mineDisrtibution[i][j] != 0)
                    {
                        mines[i][j].setText(QString::number(mineDisrtibution[i][j]));
                        numOfClicked++;
                        mines[i][j].setDisabled(true);
                    }
                    else spread(i,j);
                }
    }
    detectVictory();
}

void GamePage::spread(int x, int y)
{
    state[x][y] = 1;
    numOfClicked++;
    mines[x][y].setDisabled(true);
    mines[x][y].setStyleSheet("background-color:rgb(190,190,190);");
    for(int i = (x - 1 >= 0 ? x - 1 : 0); i < (x + 2 <= height ? x + 2 : height); i++)
        for(int j = (y - 1 >= 0 ? y - 1 : 0); j < (y + 2 <= width ? y + 2 : width); j++)
        {
            if(state[i][j] == 1)continue;
            if(mineDisrtibution[i][j] == 0)spread(i,j);
            else
            {
                state[i][j] = 1;
                numOfClicked++;
                mines[i][j].setDisabled(true);
                mines[i][j].setText(QString::number(mineDisrtibution[i][j]));
                mines[i][j].setStyleSheet("background-color:rgb(190,190,190);");
            }
        }
}

void GamePage::showAll()
{
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
        {
            if(mineDisrtibution[i][j] == -1)
            {
                mines[i][j].setText("M");
                mines[i][j].setStyleSheet("color:red;");
            }
            mines[i][j].setDisabled(true);
        }
    pauseButton->setDisabled(true);
}

void GamePage::updateLCD()
{
    millseconds += 200;
    lcd->display(millseconds / 1000);
}

void GamePage::detectVictory()
{
    if(numOfClicked == height * width - numOfMines)
    {
        QMessageBox::information(this, "Congratulations!", "You win!",QMessageBox::Ok);
        timerThread->quit();
        timerThread = nullptr;
        showAll();
    }
}

void GamePage::createTimer()
{
    timerThread = new QThread(this);
    timer = new Timer;
    timer->moveToThread(timerThread);
    timer->setInterval(200);
    connect(timerThread,SIGNAL(started()),timer,SLOT(start()));
    connect(timer,SIGNAL(timeout()),this,SLOT(updateLCD()));
    connect(pauseButton,SIGNAL(clicked(bool)),timer,SLOT(pause()));
    timerThread->start();
}

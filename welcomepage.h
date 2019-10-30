#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QPushButton>
#include <QWidget>
#include "gamepage.h"

class WelcomePage: public QWidget
{
    Q_OBJECT

public:
    WelcomePage(QWidget *parent = 0);
    ~WelcomePage();

private:
    QPushButton *easy = nullptr;
    QPushButton *intermediate = nullptr;
    QPushButton *hard = nullptr;
    GamePage *gamePage = nullptr;

private slots:
    void newGame();
};

#endif // WELCOMEPAGE_H

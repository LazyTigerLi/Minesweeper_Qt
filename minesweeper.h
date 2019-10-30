#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QWidget>
#include <QStackedLayout>
#include "welcomepage.h"

class Minesweeper : public QWidget
{
    Q_OBJECT

public:
    Minesweeper(QWidget *parent = 0);
    ~Minesweeper();
    QStackedLayout *mainLayout = nullptr;
    QSize initialSize = QSize(0,0);

private:
    WelcomePage *welcomePage = nullptr;
};

#endif // MINESWEEPER_H

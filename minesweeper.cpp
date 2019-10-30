#include "minesweeper.h"

Minesweeper::Minesweeper(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    welcomePage = new WelcomePage(this);
    mainLayout = new QStackedLayout;
    mainLayout->addWidget(welcomePage);
    setLayout(mainLayout);
}

Minesweeper::~Minesweeper()
{
}

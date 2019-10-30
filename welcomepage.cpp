#include "welcomepage.h"
#include <QVBoxLayout>
#include "minesweeper.h"

WelcomePage::WelcomePage(QWidget *parent)
    :QWidget(parent)
{
    easy = new QPushButton("初级",this);
    intermediate = new QPushButton("中级",this);
    hard = new QPushButton("高级",this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(easy);
    mainLayout->addWidget(intermediate);
    mainLayout->addWidget(hard);
    setLayout(mainLayout);

    connect(easy,SIGNAL(clicked(bool)),this,SLOT(newGame()));
    connect(intermediate,SIGNAL(clicked(bool)),this,SLOT(newGame()));
    connect(hard,SIGNAL(clicked(bool)),this,SLOT(newGame()));
}

WelcomePage::~WelcomePage()
{
}

void WelcomePage::newGame()
{
    Minesweeper *p = (Minesweeper *)parentWidget();
    p->initialSize = p->size();

    if(gamePage == nullptr)gamePage = new GamePage(p);
    QPushButton *senderButton = (QPushButton *)sender();
    if(senderButton == easy)gamePage->initialize(0);
    else if(senderButton == intermediate)gamePage->initialize(1);
    else gamePage->initialize(2);
    p->mainLayout->addWidget(gamePage);
    p->mainLayout->setCurrentWidget(gamePage);
}

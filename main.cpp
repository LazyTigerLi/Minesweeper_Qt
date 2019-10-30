#include "minesweeper.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Minesweeper *w = new Minesweeper;
    w->show();
    //Minesweeper w;
    //w.show();
    return a.exec();
}

#include "minebutton.h"

MineButton::MineButton(QWidget *parent)
    :QPushButton(parent)
{}

MineButton::~MineButton()
{}

void MineButton::mousePressEvent(QMouseEvent *e)
{
    qDebug("12345");
    int buttons = e->buttons();             //Qt::MouseButtons enum or
    if(buttons == Qt::LeftButton)emit leftClick();
    else if(buttons == Qt::RightButton)emit rightClick();
    else if(buttons == (Qt::LeftButton | Qt::RightButton))
    {
        qDebug("left and right");
        emit bothClick();
    }
}

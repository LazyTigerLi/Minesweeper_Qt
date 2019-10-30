#include "minebutton.h"

MineButton::MineButton(QWidget *parent)
    :QPushButton(parent)
{}

MineButton::~MineButton()
{}

void MineButton::mousePressEvent(QMouseEvent *e)
{
    int buttons = e->buttons();             //Qt::MouseButtons enum or
    if(buttons == Qt::LeftButton)emit leftClick();
    if(buttons == Qt::RightButton)emit rightClick();
    if(buttons == Qt::LeftButton + Qt::RightButton)emit bothClick();
}

#ifndef MINEBUTTON_H
#define MINEBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QMouseEvent>

class MineButton: public QPushButton
{
    Q_OBJECT
public:
    MineButton(QWidget *parent = 0);
    ~MineButton();

protected:
    void mousePressEvent(QMouseEvent *e);

signals:
    void leftClick();
    void rightClick();
    void bothClick();
};

#endif // MINEBUTTON_H

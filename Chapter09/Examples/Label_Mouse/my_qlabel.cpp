#include "my_qlabel.h"

my_QLabel::my_QLabel(QWidget *parent) : QLabel(parent), x(0), y(0)
{  
}

void my_QLabel::mouseMoveEvent(QMouseEvent *evt)
{
    this->x = evt->x();
    this->y = evt->y();

    //qDebug() <<" X: " << this->x << "\t Y: " << this->y << "\n";

    emit Mouse_Position();
}

void my_QLabel::mousePressEvent(QMouseEvent *evt)
{
    emit Mouse_Pressed();
}

void my_QLabel::leaveEvent(QEvent *evt)
{
   emit Mouse_Left();
}

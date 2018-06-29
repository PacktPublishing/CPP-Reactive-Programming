#ifndef GRAVITY_QLABEL_H
#define GRAVITY_QLABEL_H

#include <QLabel>

class Gravity_QLabel : public QLabel
{
   public:
    explicit Gravity_QLabel(QWidget *parent = nullptr):QLabel(parent), prev_x(0), prev_y(0){}

    int prev_x, prev_y;
};

#endif // GRAVITY_QLABEL_H

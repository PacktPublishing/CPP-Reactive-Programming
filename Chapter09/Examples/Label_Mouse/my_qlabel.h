#ifndef MY_QLABEL_H
#define MY_QLABEL_H

#include <QLabel>
#include <QMouseEvent>

class my_QLabel : public QLabel
{
    Q_OBJECT
public:
    explicit my_QLabel(QWidget *parent = nullptr);

    void mouseMoveEvent(QMouseEvent *evt);
    void mousePressEvent(QMouseEvent* evt);
    void leaveEvent(QEvent* evt);

    int x, y;

signals:
    void Mouse_Pressed();
    void Mouse_Position();
    void Mouse_Left();
};

#endif // MY_QLABEL_H

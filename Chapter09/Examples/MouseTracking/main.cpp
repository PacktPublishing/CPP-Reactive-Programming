#include <QApplication>
#include <QLabel>
#include <QMouseEvent>
#include "gravity_qlabel.h"
#include "rxqt.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto widget = new QWidget();
    widget->resize(350,300);
    widget->setCursor(Qt::OpenHandCursor);

    auto xDock = new QLabel((QWidget*)widget);
    xDock->setStyleSheet("QLabel { background-color : red}");
    xDock->resize(9,9);
    xDock->setGeometry(0, 0, 9, 9);

    auto yDock = new QLabel((QWidget*)widget);
    yDock->setStyleSheet("QLabel { background-color : blue}");
    yDock->resize(9,9);
    yDock->setGeometry(0, 0, 9, 9);

    auto gravityDock = new Gravity_QLabel((QWidget*)widget);
    gravityDock->setStyleSheet("QLabel { background-color : green}");
    gravityDock->resize(9,9);
    gravityDock->setGeometry(0, 0, 9, 9);

    rxqt::from_event(widget, QEvent::MouseButtonPress)
            .filter([](const QEvent* e) {
        auto me = static_cast<const QMouseEvent*>(e);
        return (Qt::LeftButton == me->buttons());
    })
            .subscribe([&](const QEvent* e) {
        auto me = static_cast<const QMouseEvent*>(e);
        widget->setCursor(Qt::ClosedHandCursor);
        xDock->move(me->x(), 0);
        yDock->move(0, me->y());
        gravityDock->move(me->x(),me->y());
    });


    rxqt::from_event(widget, QEvent::MouseMove)
            .filter([](const QEvent* e) {
        auto me = static_cast<const QMouseEvent*>(e);
        return (Qt::LeftButton == me->buttons());
    })
            .subscribe([&](const QEvent* e) {
        auto me = static_cast<const QMouseEvent*>(e);
        xDock->move(me->x(), 0);
        yDock->move(0, me->y());
        gravityDock->prev_x = gravityDock->prev_x * .96 + me->x() * .04;
        gravityDock->prev_y = gravityDock->prev_y * .96 + me->y() * .04;
        gravityDock->move(gravityDock->prev_x, gravityDock->prev_y);
    });


    rxqt::from_event(widget, QEvent::MouseButtonRelease)
            .subscribe([&widget](const QEvent* e) {
        widget->setCursor(Qt::OpenHandCursor);
    });

    widget->show();
    return app.exec();
}

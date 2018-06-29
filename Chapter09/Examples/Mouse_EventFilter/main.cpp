#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QMouseEvent>
#include "rx_eventfilter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Create the application window
    auto widget = std::unique_ptr<QWidget>(new QWidget());
    widget->resize(280,200);

    // Create and set properties of mouse area label
    auto label_mouseArea   = new QLabel("Mouse Area");
    label_mouseArea->setMouseTracking(true);
    label_mouseArea->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
    label_mouseArea->setFrameStyle(2);

    // Create and set properties of message display label
    auto label_coordinates = new QLabel("X = 0, Y = 0");
    label_coordinates->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
    label_coordinates->setFrameStyle(2);

    // Adjusting the size policy of widgets to allow stretching inside the vertical layout
    label_mouseArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label_coordinates->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto layout = new QVBoxLayout;
    layout->addWidget(label_mouseArea);
    layout->addWidget(label_coordinates);
    layout->setStretch(0, 4);
    layout->setStretch(1, 1);
    widget->setLayout(layout);

    // Display the mouse move message and the mouse coordinates
    rxevt::from(label_mouseArea, QEvent::MouseMove)
            .subscribe([&label_coordinates](const QEvent* e){
        auto me = static_cast<const QMouseEvent*>(e);
        label_coordinates->setText(QString("Mouse Moving : X = %1, Y = %2")
                                   .arg(me->x())
                                   .arg(me->y()));
    });

    // Display the mouse signle click message and the mouse coordinates
    rxevt::from(label_mouseArea, QEvent::MouseButtonPress)
            .subscribe([&label_coordinates](const QEvent* e){
        auto me = static_cast<const QMouseEvent*>(e);
        label_coordinates->setText(QString("Mouse Single click at X = %1, Y = %2")
                                   .arg(me->x())
                                   .arg(me->y()));
    });

    // Display the mouse double click message and the mouse coordinates
    rxevt::from(label_mouseArea, QEvent::MouseButtonDblClick)
            .subscribe([&label_coordinates](const QEvent* e){
        auto me = static_cast<const QMouseEvent*>(e);
        label_coordinates->setText(QString("Mouse Double click at X = %1, Y = %2")
                                   .arg(me->x())
                                   .arg(me->y()));
    });

    widget->show();
    return app.exec();
} // End of main

#ifndef RX_EVENTFILTER_H
#define RX_EVENTFILTER_H

#include <rxcpp/rx.hpp>
#include <QEvent>
namespace rxevt
{
    // Event filter object class
    class EventEater: public QObject
    {
    public:
        EventEater(QObject* parent, QEvent::Type type, rxcpp::subscriber<QEvent*> s):
        QObject(parent), eventType(type), eventSubscriber(s) {}

        ~EventEater()
        {
            eventSubscriber.on_completed();
        }

        bool eventFilter(QObject* obj, QEvent* event)
        {
            if(event->type() == eventType)
            {
                eventSubscriber.on_next(event);
            }
            return QObject::eventFilter(obj, event);
        }

    private:
        QEvent::Type eventType;
        rxcpp::subscriber<QEvent*> eventSubscriber;
    };

    // Utility function to retrieve the rxcpp::observable of filtered events.
    rxcpp::observable<QEvent*> from(QObject* qobject, QEvent::Type type)
    {
        if(!qobject) return rxcpp::sources::never<QEvent*>();

        return rxcpp::observable<>::create<QEvent*>(
            [qobject, type](rxcpp::subscriber<QEvent*> s) {
                qobject->installEventFilter(new EventEater(qobject, type, s));
            }
        );
    }
} // rxevt
#endif // RX_EVENTFILTER_H


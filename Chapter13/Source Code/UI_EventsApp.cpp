//--------- UI_EventsApp.cpp
#include <rxcpp/rx.hpp>
#include <cassert>
#include <cctype>
#include <clocale>

namespace Rx {
    using namespace rxcpp;
    using namespace rxcpp::sources;
    using namespace rxcpp::operators;
    using namespace rxcpp::util;
    using namespace rxcpp::subjects;
}

using namespace Rx;
using namespace std::chrono;

// Application events
enum class AppEvent {
    Active,
    Inactive,
    Data,
    Close,
    Finish,
    Other
};

int main()
{
    //-------------------
    // A or a - Active
    // I or i - Inactive
    // D or d - Data
    // C or c - Close
    // F or f - Finish
    // default - Other
    auto events = Rx::observable<>::create<AppEvent>(
        [](Rx::subscriber<AppEvent> dest) {
        std::cout << "Enter Application Events:\n";
        for (;;) {
            int key = std::cin.get();
            AppEvent current_event = AppEvent::Other;

            switch (std::tolower(key)) {
            case 'a': current_event = AppEvent::Active; break;
            case 'i': current_event = AppEvent::Inactive; break;
            case 'd': current_event = AppEvent::Data; break;
            case 'c': current_event = AppEvent::Close; break;
            case 'f': current_event = AppEvent::Finish; break;
            default:  current_event = AppEvent::Other;
            }

            if (current_event == AppEvent::Finish) {
                dest.on_completed();
                break;
            }
            else {
                dest.on_next(current_event);
            }
        }
    }).
    on_error_resume_next([](std::exception_ptr ep) {
        return rxcpp::observable<>::just(AppEvent::Finish);
    }).
    publish();

    // Event fires when application is active
    auto appActive = events.
        filter([](AppEvent const& event) {
        return event == AppEvent::Active;
    });

    // Event fires when application is inactive
    auto appInactive = events.
        filter([](AppEvent const& event) {
        return event == AppEvent::Inactive;
    });

    // Event fires when data stream starts
    auto appData = events.
        filter([](AppEvent const& event) {
        return event == AppEvent::Data;
    });

    // Event fires when application is closed
    auto appClose = events.
        filter([](AppEvent const& event) {
        return event == AppEvent::Close;
    });


    auto dataFromApp = appActive.
        map([=](AppEvent const& event) {
        std::cout << "**Application Active**\n" << std::flush;
        return appData. // Return all the data events
            take_until(appInactive). // Stop when the application goes inactive
            finally([]() {
            std::cout << "**Application Inactive**\n";
        });
    }).
        switch_on_next(). // only listen to most recent data
        take_until(appClose). // stop everything when Finish/Close event recieved
        finally([]() {
        std::cout << "**Application Close/Finish**\n";
    });

    dataFromApp.
        subscribe([](AppEvent const& event) {
        std::cout << "**Application Data**\n" << std::flush;
    });

    events.connect();

    return 0;
}
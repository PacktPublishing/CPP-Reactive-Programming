//------- OnError_ObserveOn2.cpp
#include "rxcpp/rx.hpp"
#include <mutex>

std::mutex printMutex;

int main() {

    rxcpp::rxsub::subject<int> sub;
    auto subscriber = sub.get_subscriber();
    auto observable1 = sub.get_observable();
    auto observable2 = sub.get_observable();

    auto onNext = [](int v) {
        std::lock_guard<std::mutex> lock(printMutex);
        std::cout << "Observable Thread id => "
            << std::this_thread::get_id()
            << "\tOnNext: " << v << std::endl;
    };

    auto onError = [](std::exception_ptr ep) {
        std::lock_guard<std::mutex> lock(printMutex);
        std::cout << "Observable Thread id => "
            << std::this_thread::get_id()
            << "\tOnError: "
            << rxcpp::util::what(ep).c_str() << std::endl;
    };

    //------------- Schedule it in another thread
    observable1.
        observe_on(rxcpp::synchronize_new_thread()).
        subscribe(onNext, onError,
            []() {printf("OnCompleted\n"); });

    //------------- Schedule it in yet another thread
    observable2.
        observe_on(rxcpp::synchronize_event_loop()).
        subscribe(onNext, onError,
            []() {printf("OnCompleted\n"); });

    //------------- Adding new values into the source stream
    //------------- Adding error into stream when exception occurs
    for (int i = 1; i <= 10; ++i) {
        if (i > 5) {
            try {
                std::string().at(1);
            }
            catch (...) {
                std::exception_ptr eptr = std::current_exception();
                subscriber.on_error(eptr);
            }
        }
        subscriber.on_next(i * 10);
    }
    subscriber.on_completed();

    //----------- Wait for Two Seconds
    rxcpp::observable<>::timer(std::chrono::milliseconds(2000)).
        subscribe([&](long) {});
}
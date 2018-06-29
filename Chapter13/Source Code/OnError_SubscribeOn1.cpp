//---------- SubscribeOn.cpp
#include "rxcpp/rx.hpp"
#include <thread>
#include <mutex>
//------ A global mutex for output synch.
std::mutex printMutex;

int main() {
    //-------- Creating observable streams
    auto values1 = rxcpp::observable<>::range(1, 4).
        transform([](int v) { return v * v; });

    auto values2 = rxcpp::observable<>::range(5, 9).
        transform([](int v) { return v * v; }).
        concat(rxcpp::observable<>::error<int>(std::runtime_error("Error from producer!")));

    //-------- Schedule it in another thread
    auto s1 = values1.subscribe_on(rxcpp::observe_on_event_loop());

    //-------- Schedule it in Yet another theread
    auto s2 = values2.subscribe_on(rxcpp::synchronize_new_thread());

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

    //-------- Subscribing the merged sequence
    s1.merge(s2).as_blocking().subscribe(
        onNext, onError,
        []() { std::cout << "OnCompleted" << std::endl; });

    //------------------ Print the main thread details
    std::cout << "Main Thread id => "
        << std::this_thread::get_id()
        << std::endl;
}
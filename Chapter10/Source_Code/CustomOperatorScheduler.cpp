//----------- CustomOperatorScheduler.cpp
#include "rxcpp/rx.hpp"

template <typename Duration>
auto generateObservable(Duration durarion) {
    //--------- start and the period
    auto start = rxcpp::identity_current_thread().now();
    auto period = durarion;
    //--------- Observable upto 3 items
    return rxcpp::observable<>::interval(start, period).take(3);
}

int main() {
    //-------- Create a coordination
    auto coordination = rxcpp::observe_on_event_loop();
    //-------- Instantiate a coordinator and create a worker
    auto worker = coordination.create_coordinator().get_worker();
    //----------- Create an Observable (Replay )
    auto values = generateObservable(std::chrono::milliseconds(2)).
        replay(2, coordination);
    //--------------- Subscribe first time
    worker.schedule([&](const rxcpp::schedulers::schedulable&) {
        values.subscribe([](long v) { printf("#1 -- %d : %ld\n",
            std::this_thread::get_id(), v); },
                         []() { printf("#1 --- OnCompleted\n"); });
    });
    worker.schedule([&](const rxcpp::schedulers::schedulable&) {
        values.subscribe([](long v) { printf("#2 -- %d : %ld\n",
            std::this_thread::get_id(), v); },
                         []() { printf("#2 --- OnCompleted\n"); }); });
    //----- Start the emission of values
    worker.schedule([&](const rxcpp::schedulers::schedulable&) {
        values.connect();
    });
    //------- Add blocking subscription to see results
    values.as_blocking().subscribe();
    return 0;
}
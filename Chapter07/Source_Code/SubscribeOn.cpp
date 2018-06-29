//---------- SubscribeOn.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
#include <thread>
#include <mutex>
//------ A global mutex for output synch.
std::mutex console_mutex;
//------ Print the Current Thread details
void CTDetails() {
   console_mutex.lock();
   std::cout << "Current Thread id => " 
           << std::this_thread::get_id()  << std::endl; 
   console_mutex.unlock(); 
}
//---------- a function to Yield control to other threads
void Yield( bool y ) {
   if (y) { std::this_thread::yield(); }

}

int main(){

    auto threads = rxcpp::observe_on_event_loop();

    auto values = rxcpp::observable<>::range(1);
    //------------- Schedule it in another thread
    auto s1 = values.
        subscribe_on(threads).
        map([](int prime) { CTDetails(); Yield(true); return std::make_tuple("1:", prime);});
    //-------- Schedule it in Yet another theread
    auto s2 = values.
        subscribe_on(threads). 
        map([](int prime) { CTDetails(); Yield(true) ; return std::make_tuple("2:", prime);});

    s1.merge(s2).
        take(6).as_blocking().
        subscribe(rxcpp::util::apply_to(
            [](const char* s, int p) {
                CTDetails();
                console_mutex.lock();
                printf("%s %d\n", s, p);
                console_mutex.unlock();
            }));
}

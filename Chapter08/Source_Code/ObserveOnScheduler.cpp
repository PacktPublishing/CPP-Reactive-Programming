//-------- ObservableOnScheduler.cpp
#include "rxcpp/rx.hpp"
int main(){
    //------- Print the main thread id
    printf("Main Thread Id is %d\n", 
             std::this_thread::get_id());
    //-------- We are using observe_on here
    //-------- The Map will use the main thread
    //-------- Subscribed lambda will use a new thread
    rxcpp::observable<>::range(0,15).
        map([](int i){
            printf("Map %d : %d\n", std::this_thread::get_id(),i); 
            return i;
        }).
        take(5).observe_on(rxcpp::synchronize_new_thread()).
        subscribe([&](int i){
           printf("Subs %d : %d\n", std::this_thread::get_id(),i); 
        });
    //----------- Wait for Two Seconds
    rxcpp::observable<>::timer(
       std::chrono::milliseconds(2000)).
       subscribe([&](long){ });
    
    return 0;
}
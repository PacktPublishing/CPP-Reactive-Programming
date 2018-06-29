//---------- HotObservable2.cpp
#include <rxcpp/rx.hpp>
#include <memory>

int main(int argc, char *argv[]) {

 auto eventloop = rxcpp::observe_on_event_loop();
 //----- Create a Cold Observable
 //----- Convert Cold Observable to Hot Observable 
 //----- using .publish_synchronized();
 auto values = rxcpp::observable<>::interval(
               std::chrono::seconds(2)).
               take(5).publish_synchronized(eventloop);  
 

 //----- Subscribe Twice
 values.
    subscribe(
        [](int v){printf("[1] onNext: %d\n", v);},
        [](){printf("[1] onCompleted\n");});

 values.
    subscribe(
        [](int v){printf("[2] onNext: %d\n", v);},
        [](){printf("[2] onCompleted\n");});

 //------ Start Emitting Values
 values.connect();
 //---- make a blocking subscription to see the results
 values.as_blocking().subscribe();

 //----------- Wait for Two Seconds
 rxcpp::observable<>::timer(
       std::chrono::milliseconds(2000)).
       subscribe([&](long){ });

}



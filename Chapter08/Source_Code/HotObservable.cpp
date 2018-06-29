//---------- HotObservable.cpp
#include <rxcpp/rx.hpp>
#include <memory>

int main(int argc, char *argv[]) {

 auto eventloop = rxcpp::observe_on_event_loop();
 //----- Create a Cold Observable
 //----- Convert Cold Observable to Hot Observable 
 //----- using .Publish();
 auto values = rxcpp::observable<>::interval(
               std::chrono::seconds(2)).take(2).publish();  
  
 //----- Subscribe Twice
 values.
    subscribe_on(eventloop).
    subscribe(
        [](int v){printf("[1] onNext: %d\n", v);},
        [](){printf("[1] onCompleted\n");});

 values.
    subscribe_on(eventloop).
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



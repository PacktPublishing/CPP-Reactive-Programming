//------------- ReplaySubject.cpp
#include <rxcpp/rx.hpp>
#include <memory>
int main(int argc, char *argv[]) {

    //----------- instantiate a ReplaySubject
    rxcpp::subjects::replay<int,rxcpp::observe_on_one_worker>      
           replay_subject(10,rxcpp::observe_on_new_thread());

    //---------- get the observable interface
    auto observable = replay_subject.get_observable();
    //---------- Subscribe!
    observable.subscribe( [] ( int v ) {
        printf("1------%d\n",v );
     });
    //--------- get the subscriber interface
    auto subscriber = replay_subject.get_subscriber();
    //---------- Emit data 
    subscriber.on_next(1);
    subscriber.on_next(2);
    //-------- Add a new subscriber
    //-------- A normal subject will drop data
    //-------- Replay subject will not
    observable.subscribe( [] ( int v ) {
        printf("2------%d\n",v );
     });

    //----------- Wait for Two Seconds
    rxcpp::observable<>::timer(
       std::chrono::milliseconds(2000)).
       subscribe([&](long){ });

}


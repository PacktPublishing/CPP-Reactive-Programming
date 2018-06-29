//-------- BehaviorSubject.cpp
#include <rxcpp/rx.hpp>
#include <memory>

int main(int argc, char *argv[]) {

    rxcpp::subjects::behavior<int> behsubject(0);

    auto observable = behsubject.get_observable();
    observable.subscribe( [] ( int v ) {
        printf("1------%d\n",v );
     });

      observable.subscribe( [] ( int v ) {
        printf("2------%d\n",v );
     });

    auto subscriber = behsubject.get_subscriber();
    subscriber.on_next(1);
    subscriber.on_next(2);

    int n = behsubject.get_value();

    printf ("Last Value ....%d\n",n);

}


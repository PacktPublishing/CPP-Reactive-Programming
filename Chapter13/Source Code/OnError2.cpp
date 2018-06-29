//------- OnError2.cpp
#include "rxcpp/rx.hpp"


int main() {
    rxcpp::rxsub::subject<int> sub;
    auto subscriber = sub.get_subscriber();
    auto observable = sub.get_observable();
    
    observable.subscribe(
                     [](int v){printf("OnNext: %d\n", v);},
                     [](std::exception_ptr ep){
                         printf("OnError: %s\n", rxcpp::util::what(ep).c_str());
                     },
                     [](){printf("OnCompleted\n");});
    
    
    for (int i = 1; i <= 10; ++i) {
        if(i > 5) {
            try {
                std::string().at(1);
            } catch (...) {
                std::exception_ptr eptr = std::current_exception();
                subscriber.on_error(eptr);
            }
        }
        subscriber.on_next(i * 10);
    }
    subscriber.on_completed();
}

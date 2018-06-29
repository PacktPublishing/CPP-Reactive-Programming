#include "rxcpp/rx.hpp"

int main() {
    auto ints = rxcpp::observable<>::create<int>( [](rxcpp::subscriber<int> s) {
        for(auto i = 1; i <=5; ++i){
            s.on_next(i);
            if( i > 4) {
                try {
                    std::string().at(1);
                } catch (...) {
                    std::exception_ptr eptr = std::current_exception();
                    s.on_error(eptr);
                    break;
                }
            }
        }
        s.on_completed();
    });
    
    ints.
    subscribe(
              [](int v){printf("OnNext: %d\n", v);},
              [](std::exception_ptr ep){
                  printf("OnError: %s\n", rxcpp::util::what(ep).c_str()); },
              [](){printf("OnCompleted\n");});
}

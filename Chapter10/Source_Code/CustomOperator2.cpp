//------ CustomOperator2.cpp
#include "rxcpp/rx.hpp"

namespace rx {
    using namespace rxcpp;
    using namespace rxcpp::operators;
    using namespace rxcpp::sources;
    using namespace rxcpp::util;
}

std::function<rx::observable<int>(rx::observable<int>)> getOddNumSquare() {
    return [](rx::observable<int> $str) {
        return $str.
        filter([](int v){
            return v%2;
        }).
        map([](const int v) {
            return v*v;
        }).
        take(3)|
        //------ Translating exception
        rx::on_error_resume_next([](std::exception_ptr){
            return rx::error<int>(std::runtime_error("custom exception"));
        });
    };
}

int main() {
    //------ Create an observable composing the custom operator
    auto value = rxcpp::observable<>::range(1, 7) |
    getOddNumSquare();
    
    value.
    subscribe(
              [](int v){printf("OnNext: %d\n", v);},
              [](){printf("OnCompleted\n");});
}

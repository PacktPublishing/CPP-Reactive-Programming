//----------- DeferExample.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
int main() {
    auto observable_factory = [](){
         return rxcpp::observable<>::range(1,3).
                 map( [] ( int n  ) {return n*n; });
    };
    auto ints = rxcpp::observable<>::defer(observable_factory);
    ints.
        subscribe(
            [](int v){printf("OnNext: %d\n", v);},
            [](){printf("OnCompleted\n");});

    ints.
        subscribe(
            [](int v){printf("2nd OnNext: %d\n", v);},
            [](){printf("2nd OnCompleted\n");});
}
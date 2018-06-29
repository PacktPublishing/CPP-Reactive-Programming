//------------------ Map.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"

#include <iostream>

#include <array>

int main() {

    auto ints = rxcpp::observable<>::range(1,10).
                 map( [] ( int n  ) {return n*n; });

    ints.subscribe(
            [](int v){printf("OnNext: %d\n", v);},
            [](){printf("OnCompleted\n");});

}

//------------------ Map_With_Pipe.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
namespace Rx {
using namespace rxcpp;
using namespace rxcpp::sources;
using namespace rxcpp::operators;
using namespace rxcpp::util;
}
using namespace Rx;
#include <iostream>



int main() {

    auto ints = rxcpp::observable<>::range(1,10) | 
                 map( [] ( int n  ) {return n*n; });

    ints | subscribe(
            [](int v){printf("OnNext: %d\n", v);},
            [](){printf("OnCompleted\n");});

}

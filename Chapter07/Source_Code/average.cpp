//----------- Average.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>

int main() {

    auto values = rxcpp::observable<>::range(1, 20).average();
    values.
        subscribe(
            [](double v){printf("average: %lf\n", v);},
            [](){printf("OnCompleted\n");});

}

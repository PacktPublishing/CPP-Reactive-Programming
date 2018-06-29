//----------- BufferExample.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
int main() {


    auto values = rxcpp::observable<>::range(1, 10).buffer(2);
    values.
        subscribe(
            [](std::vector<int> v){
                printf("OnNext:{");
                std::for_each(v.begin(), v.end(), [](int a){
                    printf(" %d", a);
                });
                printf("}\n");
            },
            [](){printf("OnCompleted\n");});

}
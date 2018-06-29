//---------------- Unsubscribe.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
int main() {

    auto subs = rxcpp::composite_subscription();
    auto values = rxcpp::observable<>::range(1, 10);
    values.subscribe(
        subs,
        [&subs](int v){
            printf("OnNext: %d\n", v);
            if (v == 6)
                subs.unsubscribe(); //-- Stop recieving events
        },
        [](){printf("OnCompleted\n");});

}
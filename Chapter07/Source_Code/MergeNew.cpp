//---------------- Merge.cpp
#include "rxcpp/rx.hpp"
#include "rxcpp/rx-test.hpp"
#include <iostream>
#include <array>
int main() {
    auto o1 = rxcpp::observable<>::range(1, 3);
    auto o2 = rxcpp::observable<>::from(4,5, 6);
    auto values = o1.merge(o2);
    values.subscribe(
            [](int v){printf("OnNext: %d\n", v);},[](){printf("OnCompleted\n");});
}

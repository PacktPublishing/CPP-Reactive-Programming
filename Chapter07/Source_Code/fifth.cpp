#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"

#include <iostream>

#include <array>

int main() {

 auto values = rxcpp::observable<>::range(1); // infinite (until overflow) stream of integers

    auto s1 = values.
        take(3).
        map([](int prime) { return std::make_tuple("1:", prime);});

    auto s2 = values.
        take(3).
        map([](int prime) { return std::make_tuple("2:", prime);});

    s1.
        concat(s2).
        subscribe(rxcpp::util::apply_to(
            [](const char* s, int p) {
                printf("%s %d\n", s, p);
            }));


}

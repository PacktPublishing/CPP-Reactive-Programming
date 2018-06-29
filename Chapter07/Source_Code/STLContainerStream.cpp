#include "rxcpp/rx.hpp"

#include <iostream>

#include <array>

int main() {

 std::array< int, 3 > a={{1, 2, 3}};
    auto values1 = rxcpp::observable<>::iterate(a);
    values1.
        subscribe(
            [](int v){printf("OnNext: %d\n", v);},
            [](){printf("OnCompleted\n");});


}

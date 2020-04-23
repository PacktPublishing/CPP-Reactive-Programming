//----- OperatorChaining1.cpp
//----- Square and multiplication by 2 in order
#include "rxcpp/rx.hpp"

int main()
{
    auto values = rxcpp::observable<>::range(1, 3).
        map([](int x) { return x * x; }).
        map([](int x) { return x * 2; });

    values.subscribe(
        [](int v) {printf("OnNext: %d\n", v); },
        []() {printf("OnCompleted\n"); });

    return 0;
}

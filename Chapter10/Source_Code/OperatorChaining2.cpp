//----- OperatorChaining2.cpp
//----- Multiplication by 2 and Square in order
#include "rxcpp/rx.hpp"

int main()
{
    auto values = rxcpp::observable<>::range(1, 3).
        map([](int x) { return x * 2; }).
        map([](int x) { return x * x; });

    values.subscribe(
        [](int v) {printf("OnNext: %d\n", v); },
        []() {printf("OnCompleted\n"); });

    return 0;
}

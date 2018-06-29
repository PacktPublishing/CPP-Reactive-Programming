//------- Finally.cpp
#include "rxcpp/rx.hpp"

int main()
{
    auto values = rxcpp::observable<>::range(1, 3).
        concat(rxcpp::observable<>::error<int>(std::runtime_error("Error from producer!"))).
        //----- Final action
        finally([]() { printf("The final action\n");
    });

    values.
        subscribe(
            [](int v) {printf("OnNext: %d\n", v); },
            [](std::exception_ptr ep) {
        printf("OnError: %s\n", rxcpp::util::what(ep).c_str()); },
            []() {printf("OnCompleted\n"); });
}
//------ OnError1
#include "rxcpp/rx.hpp"

int main()
{
    //------ Creating observable with an error appended
    auto vals = rxcpp::observable<>::range(1, 3).
        concat(rxcpp::observable<>::error<int>(std::runtime_error("Error from producer!")));
    vals.
        subscribe(
            [](int v) {printf("OnNext: %d\n", v); },
            [](std::exception_ptr ep) {
        printf("OnError: %s\n", rxcpp::util::what(ep).c_str());
    },
            []() {printf("OnCompleted\n"); });
}
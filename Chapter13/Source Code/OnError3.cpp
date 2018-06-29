//------- OnError3.cpp
#include "rxcpp/rx.hpp"

int main()
{
    //------- Create an observable with appended error
    auto values = rxcpp::observable<>::range(1, 3).
        concat(rxcpp::observable<>::error<int>(std::runtime_error("Error from producer!"))).
        //------- Resuming with another stream
        on_error_resume_next([](std::exception_ptr ep) {
        printf("Resuming after: %s\n", rxcpp::util::what(ep).c_str());
        return rxcpp::observable<>::just(-1); //return rxcpp::observable<>::range(4,6);
    });

    values.
        subscribe(
            [](int v) {printf("OnNext: %d\n", v); },
            [](std::exception_ptr ep) {
                printf("OnError: %s\n", rxcpp::util::what(ep).c_str()); },
            []() {printf("OnCompleted\n"); });
}

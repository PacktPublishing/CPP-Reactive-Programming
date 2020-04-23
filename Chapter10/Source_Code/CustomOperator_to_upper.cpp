//----------- CustomOperator_to_upper.cpp
#include "rxcpp/rx.hpp"

namespace rx {
    using namespace rxcpp;
    using namespace rxcpp::operators;
    using namespace rxcpp::sources;
    using namespace rxcpp::util;
}

template<typename Container>
rx::observable<std::string> to_upper(Container items) {
    auto str = rx::observable<>::iterate(items);
    return str.
    map([](std::string s){
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    }).
    //------ Translating exception
    on_error_resume_next([](std::exception_ptr){
        return rx::error<std::string>(std::runtime_error("custom exception"));
    });
}

int main() {
    //------ Create an observable composing the custom operator
    auto names = {"delhi", "mumbai", "chennai", "kolkata"};
    auto value = to_upper(names).take(3);
    
    auto error_handler = [=](std::exception_ptr e) {
        try { rethrow_exception(e); }
        catch (const std::exception &ex) {
            std::cerr << ex.what() << std::endl;
        }
    };
    
    value.
    subscribe(
              [](std::string s){printf("OnNext: %s\n", s.c_str());},
              error_handler,
              [](){printf("OnCompleted\n");});
}

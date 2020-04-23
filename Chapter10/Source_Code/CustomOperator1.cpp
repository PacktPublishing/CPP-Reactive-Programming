//------ CustomOperator1.cpp
#include "rxcpp/rx.hpp"

namespace rx {
    using namespace rxcpp;
    using namespace rxcpp::operators;
    using namespace rxcpp::sources;
    using namespace rxcpp::util;
}


template<typename Container>
rx::observable<std::string> helloNames(Container items) {
    auto str = rx::observable<>::iterate(items);
    return str.
    filter([](std::string s){
        return s.length() > 5;
    }).
    map([](std::string s){
        return "Hello, " + s + "!";
    }).
    //------ Translating exception
    on_error_resume_next([](std::exception_ptr){
        return rx::error<std::string>(std::runtime_error("custom exception"));
    });
}

int main() {
    //------ Create an observable composing the custom operator
    auto names = {"Praseed", "Peter", "Joseph", "Jai"};
    auto value = helloNames(names).take(2);
    
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



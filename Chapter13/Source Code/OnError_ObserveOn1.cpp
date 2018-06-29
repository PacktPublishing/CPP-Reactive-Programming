//----------OnError_ObserveOn1.cpp
#include "rxcpp/rx.hpp"
#include <iostream>
#include <thread>

int main(){
    //---------------- Generate a range of values
    //---------------- Apply Square function
    auto values = rxcpp::observable<>::range(1,4).
    map([](int v){ return v*v;}).
    concat(rxcpp::observable<>::error<int>(std::runtime_error("Error from producer!")));
    
    //------------- Emit the current thread details
    std::cout  << "Main Thread id => "
    << std::this_thread::get_id()
    << std::endl;

    //---------- observe_on another thread....
    //---------- make it blocking too
    values.observe_on(rxcpp::synchronize_new_thread()).as_blocking().
    subscribe([](int v){
        std::cout << "Observable Thread id => " 
                  << std::this_thread::get_id()
                  <<" "<<v<<std::endl;},
              [](std::exception_ptr ep) { 
                printf("OnError: %s\n", rxcpp::util::what(ep).c_str()); },
              [](){ std::cout << "OnCompleted" << std::endl; });

    //------------------ Print the main thread details
    std::cout << "Main Thread id => "
    << std::this_thread::get_id()
    << std::endl;
}

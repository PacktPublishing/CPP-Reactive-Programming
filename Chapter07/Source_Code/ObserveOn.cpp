//----------ObserveOn.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
#include <thread>

int main(){

 //---------------- Generate a range of values
 //---------------- Apply Square function
 auto values = rxcpp::observable<>::range(1,4).
               map([](int v){
            
                    return v*v;
        
               });
   
 //------------- Emit the current thread details
 std::cout  << "Main Thread id => " 
            << std::this_thread::get_id() 
            << std::endl;
 //---------- observe_on another thread....
 //---------- make it blocking to 
 values.observe_on(rxcpp::synchronize_new_thread()).
        
             as_blocking().
subscribe(
           
                   [](int v){ 
                   std::cout << "Observable Thread id => " 
                             << std::this_thread::get_id() 
                             << "  " << v << std::endl ;},
        
                   [](){ std::cout << "OnCompleted" << std::endl; });
    
 //------------------ Print the main thread details
 std::cout << "Main Thread id => " 
           << std::this_thread::get_id() 
           << std::endl;  

}

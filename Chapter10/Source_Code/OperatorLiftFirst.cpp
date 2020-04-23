//----------- OperatorLiftFirst.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
namespace rxu=rxcpp::util;

#include <array>

using namespace rxcpp;
using namespace rxcpp::operators;




int main() {
     std::array< int ,4 > a={{10, 20,30,40}};
  
     auto lambda_fn = [] ( int n ) { return n*2; };
     auto transform = [=](rxcpp::subscriber<int> dest){
         return rxcpp::make_subscriber<int>(
                dest,rxcpp::make_observer_dynamic<int>(
                      [=](int n){
                         dest.on_next(lambda_fn(n));
                      },
                      [=]( std::exception_ptr e ){dest.on_error(e);},
                      [=](){dest.on_completed();}));
     };
     // type of values will be observable<string>
     // Lazy Evaluation 
     auto values = rxcpp::observable<>::iterate(a); 

    
     //-------- As usual subscribe 
     values.lift<int>(transform).subscribe( 
              [] (int f) { std::cout << f <<  std::endl; } , 
              [] () {std::cout << "Hello World.." << std::endl;} );
}
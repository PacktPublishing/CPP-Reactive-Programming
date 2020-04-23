//----------- OperatorCompose.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
namespace rxu=rxcpp::util;

#include <array>

using namespace rxcpp;
using namespace rxcpp::operators;

int main() {
     std::array< int ,4 > a={{10, 20,30,40}};

     // h-function
     auto h = [] (observable<int> src ) {
       return src.map([](int n ) { return n; });
     };
     // g-function
     auto g = [] (observable<int> src ) {

        return src.map([](int n ) { return n*2; });
             
     };
     // type of values will be observable<string>
     // Lazy Evaluation ... apply h over observable<string>
     // on the result, apply g 
     auto values = g(h(rxcpp::observable<>::iterate(a))); 
     //-------- As usual subscribe 
     values.subscribe( 
              [] (int f) { std::cout << f <<  std::endl; } , 
              [] () {std::cout << "Hello World.." << std::endl;} );
}
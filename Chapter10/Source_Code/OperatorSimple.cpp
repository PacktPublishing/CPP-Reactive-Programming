//----------- OperatorSimple.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
namespace rxu=rxcpp::util;

#include <array>

using namespace rxcpp;
using namespace rxcpp::operators;

///////////////////////////////
// Write a Simple Reactive Operator
// Takes an Observable<string> and
// Prefix Hello to every item and return
// another Observable<string>
observable<std::string> helloNames(observable<std::string> src ) {
    
    return src.map([](std::string s) { 
             return "Hello, " + s + "!"; 
    });
}

int main() {
     std::array< std::string,4 > a={{"Praseed", "Peter", "Sanjay","Raju"}};
     // type of values will be observable<string>
     // Lazy Evaluation 
     auto values = helloNames(rxcpp::observable<>::iterate(a)); 
     //-------- As usual subscribe 
     values.subscribe( 
              [] (std::string f) { std::cout << f <<  std::endl; } , 
              [] () {std::cout << "Hello World.." << std::endl;} );
}
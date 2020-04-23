//----------- OperatorCompose.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
namespace rxu=rxcpp::util;

#include <array>
using namespace std;
using namespace rxcpp;
using namespace rxcpp::operators;

int main() {
     std::array< string ,4 > a={{"Bjarne","Kirk","Herb","Sean"}};
     auto h = [] (observable<string> src ) {

        return src.eval([](string s ) { return s+"!"; });
             
     };

     //-------- We will Lift g using eval
     auto g = [](string s) { return "Hello : " + s; };
             
    
     // use apply h first and then call eval
     auto values = h(rxcpp::observable<>::iterate(a)).
                   eval(g); 
     //-------- As usual subscribe 
     values.subscribe( 
              [] (string f) { std::cout << f <<  std::endl; } , 
              [] () {std::cout << "Hello World.." << std::endl;} );
}
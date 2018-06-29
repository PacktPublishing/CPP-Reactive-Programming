//----------- ConcatMap.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
namespace rxu=rxcpp::util;

#include <array>
int main() {

     std::array< std::string,4 > a={{"Praseed", "Peter", "Sanjay","Raju"}};
     //---------- Apply Concat map on the array of names
     //---------- Concat Map returns an Observable<T> ( oncat returns T )
     //---------- The First lamda creates a new Observable<T>
     //---------- The Second Lambda manipulates primary observable and 
     //---------- Concatenated Observable
     auto values = rxcpp::observable<>::iterate(a).concat_map(
              [] (std::string v ) {
                   std::array<std::string,3> salutation=
                       { { "Mr." ,  "Monsieur" , "Sri" }};
                   return rxcpp::observable<>::iterate(salutation);
              },
              [] ( std::string f , std::string s ) {return s + " " +f;});

     //-------- As usual subscribe 
     //-------- Here the value will be interleaved as concat_map concats the 
     //-------- Two streams
     values.subscribe( 
              [] (std::string f) { std::cout << f <<  std::endl; } , 
              [] () {std::cout << "Hello World.." << std::endl;} );
    

}
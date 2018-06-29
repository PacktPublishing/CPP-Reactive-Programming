#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>

namespace rxu=rxcpp::util;


#include <array>
//#include <tuple>




int main() {

     std::array< std::string,4 > a={{"Praseed", "Peter", "Sanjay","Raju"}};
     auto values = rxcpp::observable<>::iterate(a).flat_map(
              [] (std::string v ) {
                   std::array<std::string,3> salutation ={ { "Mr." ,  "Monsieur" , "Sri" }};
                   return rxcpp::observable<>::iterate(salutation);
              },
              [] ( std::string f , std::string s ) {

			
                        return s + " " +f;

              });

      
     values.subscribe( [] (std::string f) {
              std::cout << f <<  std::endl; } , [] () {std::cout << "Hello World.." << std::endl;} );
    

}
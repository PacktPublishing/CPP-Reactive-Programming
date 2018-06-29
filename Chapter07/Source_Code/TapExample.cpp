//----------- TapExample.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
int main() {

     //---- Create a mapped Observable
     auto ints = rxcpp::observable<>::range(1,3).
                 map( [] ( int n  ) {return n*n; });
     //---- Apply the tap operator...The Operator 
     //---- will act as a filter/debug operator
     auto values = ints.

          tap(
[](int v)
           {printf("Tap -       OnNext: %d\n", v);},

            [](){printf("Tap -       OnCompleted\n");
          });
    
     //------- Do some action
     values.
subscribe(

          [](int v){printf("Subscribe - OnNext: %d\n", v);},
 
          [](){printf("Subscribe - OnCompleted\n");});

}
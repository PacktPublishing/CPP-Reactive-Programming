#include "rxcpp/rx.hpp"

#include <iostream>

#include <array>

int main() {

  auto values = rxcpp::observable<>::range(1, 12).

                filter([](int v){
 return v % 2 == 0;
}).map([](int x) {return x*x;}); 
  values.
subscribe(
 [](int v){printf("OnNext: %d\n", v);},
  
                [](){printf("OnCompleted\n");});


}

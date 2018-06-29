//////////
//first.cpp
// g++ -I<PathToRxcpplibfoldersrc> First.cpp
//
#include "rxcpp/rx.hpp"
#include <iostream>

int main() {

 //------------- Create an Observable.. a stream of numbers
 auto observable = rxcpp::observable<>::range(1, 12);
     

 //------------ Subscribe (only OnNext and OnCompleted Lambda given
 observable.
subscribe(
           
    [](int v){printf("OnNext: %d\n", v);},
            
    [](){printf("OnCompleted\n");});

}

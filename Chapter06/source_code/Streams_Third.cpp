//--------------- Streams_Third.cpp
// g++ -I./Streams-master/sources Streams_Third.cpp
//
#include "Stream.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
using namespace std;
using namespace stream;
using namespace stream::op;


double square( double a ) { return a*a; }

int main() {
  std::vector<double> values = { 1,2,3,4,5 };
 
  std::vector<double> outputs = MakeStream::from(values)
               | map_([] (double a ) { return a*a;}) 
               | to_vector();

  
  for(auto pn : outputs )
    cout << pn << endl;

}
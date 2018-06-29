//--------------- Streams_Second.cpp
// g++ -I./Streams-master/sources Streams_Second.cpp
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

double AMEAN( const std::vector<double>& arr )
{
   double len = arr.size();
   auto sm = MakeStream::from(std::begin(arr),std::end(arr)) 
             | reduce(std::plus<void>());
   return sm/len;
}

double STD( const std::vector<double>& arr ) {
   
   double len = arr.size();
   double t = AMEAN(arr);
   double sum = MakeStream::from(std::begin(arr),std::end(arr)) | reduce( 
         [=] ( double an, double bn )  { return (an-t)*(an-t) +bn; } );
   return sqrt(sum/len);
}

double GMEAN( const std::vector<double>& arr ) {
   
   double len = arr.size();
   double product = MakeStream::from(std::begin(arr),std::end(arr)) | identity_reduce(1,std::multiplies<double> {} );
   return exp( (1.0/len)*log(product));
        

}


int main() {

  std::vector<double> a = { 10,20,30,40,50 };
  //------------ Make a Stream and reduce 
  auto val =  MakeStream::from(a) 
              | reduce(std::plus<void>());
  //------ Compute the arithematic average
  cout << val/a.size() << endl;

  //------------------- The same stuff can be achieved through the below
  val = MakeStream::from(a) | reduce( [] ( double an, double bn ) { return an+bn; } );
  cout << val/a.size() << endl;

}
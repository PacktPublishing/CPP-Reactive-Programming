//------------  Variant.cpp
//------------- g++ -std=c++1z Variant.cpp
#include <variant>
#include <string>
#include <cassert>
#include <iostream>
using namespace std;
int main(){
    std::variant<int, float,string> v, w;
    v = 12.0f; // v contains now contains float 
    cout <<  std::get<1>(v) << endl;
    w = 20; // assign to int
    cout <<  std::get<0>(w) << endl;
    w = "hello"s; //assign to string
    cout <<  std::get<2>(w) << endl;
}

//------- RvaluerefCall.cpp
#include <iostream>
using namespace std;
void TestFunction( int & a ) {cout << a << endl;}
void TestFunction( int && a ){
   cout << "rvalue references" << endl;
   cout << a << endl;
}
int main() {
  int&& j = 42;
  int x = 3,y=5;
  int&& z = x + y;
  TestFunction(x + y ); // Should call Rvalue reference function
  TestFunction(j); // Calls Lvalue Refreence function
}

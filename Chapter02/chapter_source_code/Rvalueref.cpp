///---- Rvaluref.cpp
#include <iostream>
using namespace std;
int main() {
  int&& j = 42;int x = 3,y=5; int&& z = x + y;  cout << z << endl;
  z = 10; cout << z << endl;j=20;cout << j << endl;
}

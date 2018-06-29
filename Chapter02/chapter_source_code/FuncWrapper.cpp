//---------------- FuncWrapper.cpp  Requires C++ 17 (-std=c++1z )
#include <functional>
#include <iostream>
using namespace std;
//-------------- Simple Function call
void PrintNumber(int val){ cout << val << endl; }
// ------------------ A class which overloads function operator
struct PrintNumber {
   void operator()(int i) const { std::cout << i << '\n';}
};
//------------ To demonstrate the usage of method call
struct FooClass {
   int number;
   FooClass(int pnum) : number(pnum){}
   void PrintNumber(int val) const  { std::cout << number + val<< endl; }
};
int main() {
	// ----------------- Ordinary Function Wrapped
	std::function<void(int)> displaynum = PrintNumber;
	displaynum(0xF000);
       std::invoke(displaynum,0xFF00); //call through std::invoke
	//-------------- Lambda Functions Wrapped
	std::function<void()> lambdaprint = []() { PrintNumber(786); };
       	 lambdaprint();
       	 std::invoke(lambdaprint);
              // Wrapping member functions of a class 
              std::function<void(const FooClass&, int)> 
              classdisplay = &FooClass::PrintNumber;
              //  creating an instance
              const FooClass fooinstance(100); 
              classdisplay (fooinstance,100); 
}

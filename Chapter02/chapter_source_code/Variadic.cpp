//Variadic.cpp
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;
//--- add given below is a base case for ending compile time
//--- recursion
int add() { return 0; } // end condition
//---- Declare a Variadic function Template
//----  … is called parameter pack. The compiler 
//---  synthesize a function based on the number of arguemtns
//------ give by the programmer.
template<class T0, class ... Ts>
decltype(auto) add(T0 first, Ts ... rest) {
	return first + add(rest ...);
}
int main() { int n = add(0,2,3,4); cout << n << endl; }

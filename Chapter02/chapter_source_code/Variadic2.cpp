//Variadic2.cpp
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;
//--------- Print values to the console for basic types
//--------  These are base case versions
void EmitConsole(int value) { cout << "Integer: " << value << endl; }
void EmitConsole(double value) { cout << "Double: " << value << endl; }
void EmitConsole(const string& value){cout << "String: "<<value<< endl; }

template<typename T>
void EmitValues(T&& arg) { EmitConsole(std::forward<T>(arg)); }

template<typename T1, typename... Tn>
void EmitValues(T1&& arg1, Tn&&... args){
	EmitConsole(std::forward<T1>(arg1));
	EmitValues(std::forward<Tn>(args)...);
}

int main() {  EmitValues(0,2.0,"Hello World",4); }

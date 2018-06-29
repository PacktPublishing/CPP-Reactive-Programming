//---- PipeFunc2.cpp 
//---- g++ -std=c++1z PipeFunc2.cpp
#include <iostream>
using namespace std;

struct AddOne {
    template<class T>
    auto operator()(T x) const { return x + 1; }
};
struct SumFunction {
    template<class T>
    auto operator()(T x,T y) const   { return x + y;} // Binary Operator
};


//-------------- Create a Pipable Closure Function (Unary)
//-------------- Uses Variadic Templates Paramter pack
template<class F>
struct PipableClosure : F{
    template<class... Xs>
    PipableClosure(Xs&&... xs) : // Xs is a universal reference
	F(std::forward<Xs>(xs)...) // perfect forwarding 
    {}
};
//---------- A helper function which converts a Function to a Closure
template<class F>
auto MakePipeClosure(F f) 
{ return PipableClosure<F>(std::move(f)); }
// ------------ Declare a Closure for Binary
//------------- Functions 
//
template<class F>
struct PipableClosureBinary {
    template<class... Ts>
    auto operator()(Ts... xs) const {
        return MakePipeClosure([=](auto x) -> decltype(auto) 
        { return F()(x, xs...);}); }
};
//------- Declare a pipe operator
//------- uses perfect forwarding to invoke the function
template<class T, class F> //---- Declare a pipe operator
decltype(auto) operator|(T&& x, const PipableClosure<F>& pfn)
{ return pfn(std::forward<T>(x)); }



int main() {
	//-------- Declare a Unary Function Closure
	const PipableClosure<AddOne> fnclosure = {};
	int value = 1 | fnclosure| fnclosure;
	std::cout << value << std::endl;    
           
       //--------- Decalre a Binary function closure
       const PipableClosureBinary<SumFunction> sumfunction = {};
	int value1 = 1 | sumfunction(2) | sumfunction(5) | fnclosure;
	std::cout << value1 << std::endl;
}


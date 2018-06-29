//------------ Compose.cpp
//------ g++ -std=c++ Compose.cpp
#include <iostream>
using namespace std;
//---------- base case compile time recursion
//---------- stops here
template <typename F, typename G>
auto Compose(F&& f, G&& g)
{ return [=](auto x) { return f(g(x)); };}
//----- Performs compile time recursion based
//----- on number of parameters
template <typename F, typename... R>
auto Compose(F&& f, R&&... r){
	return [=](auto x) { return f(Compose(r...)(x)); };
}

auto CurriedAdd3(int x) {
    return [x](int y) { //capture x
        return [x, y](int z){ return x + y + z; };
    };
};

template <typename... Ts>
auto PartialFunctionAdd3(Ts... xs) {
    static_assert(sizeof...(xs) <= 3);

    if constexpr (sizeof...(xs) == 3){
        // Base case: evaluate and return the sum.
        return (0 + ... + xs);
    }
    else{
        // Recursive case: bind `xs...` and return another 
        return [xs...](auto... ys){
            return PartialFunctionAdd3(xs..., ys...);
        };
    }
}


int main() {
       // ------------- Compose two functions together
auto val = Compose(
[](int const a) {return std::to_string(a); },
[](int const a) {return a * a; })(4); // val = "16"
cout << val << std::endl; //should print 16
	// ----------------- Invoke the Curried function
auto p = CurriedAdd3(4)(5)(6); 
cout << p << endl;
//-------------- Compose a set of function together

auto func = Compose(
[](int const n) {return std::to_string(n); },
[](int const n) {return n * n; },
[](int const n) {return n + n; },
[](int const n) {return std::abs(n); }); 
cout << func(5) << endl;
       //----------- Invoke Partial Functions giving different arguments
PartialFunctionAdd3(1, 2, 3); 
PartialFunctionAdd3(1, 2)(3); 
PartialFunctionAdd3(1)(2)(3); 

}

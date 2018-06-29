//---------------- Folds.cpp
//--------------- Requires C++ 17 (-std=c++1z )
#include <functional>
#include <iostream>

using namespace std;
template <typename... Ts>
auto AddFoldLeftUn(Ts... args) { return (... + args); }
template <typename... Ts>
auto AddFoldLeftBin(int n,Ts... args){ return (n + ... + args);}
template <typename... Ts>
auto AddFoldRightUn(Ts... args) { return (args + ...); }
template <typename... Ts>
auto AddFoldRightBin(int n,Ts... args) { return (args + ... + n); }
template <typename T,typename... Ts>
auto AddFoldRightBinPoly(T n,Ts... args) { return (args + ... + n); }

int main() {
        auto a = AddFoldLeftUn(1,2,3,4);
        cout << a << endl;
        cout << AddFoldRightBin(a,4,5,6) << endl;
        auto b =  AddFoldRightBinPoly("C++ "s,"Hello "s,"World "s );
        cout << b <<  endl;
}

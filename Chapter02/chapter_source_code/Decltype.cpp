//---- Decltype.cpp
#include <iostream>
using namespace std;
int foo()   { return 10;  }
char bar()  { return 'g'; }
auto fancy() -> decltype(1.0f) { return 1;} //return type is float
int main() {
    // Data type of x is same as return type of foo()
    // and type of y is same as return type of bar()
    decltype(foo()) x;
    decltype(bar()) y;
    cout <<  typeid(x).name() << endl;
    cout <<  typeid(y).name() << endl;
    struct A { double x; };
    const A* a = new A();
    decltype(a->x) z; // type is double
    decltype((a->x)) t= z; // type is const double&
    cout <<  typeid(z).name() << endl;
    cout <<  typeid(t).name() << endl;
    cout <<  typeid(decltype(fancy())).name() << endl;
    return 0;
}

//----- LambdaThird.cpp
#include <iostream>
#include <numeric>
using namespace std;
//////////////////////////
// Functors to add and multiply  two numbers
template <typename T>
struct addition{
T operator () (const T& init,  const T& a ) { return init + a; }
};
template <typename T>
struct multiply {
 T operator () (const T& init, const T& a ) { return init * a; }
};

int main()
{
   double v1[3] = {1.0, 2.0, 4.0}, sum;
   sum = accumulate(v1, v1 + 3, 0.0, addition<double>());
   cout << "sum = " << sum << endl;
   sum = accumulate(v1,v1+3,0.0, [] (const double& a ,const double& b ) {
           return a +b;
   }); 
   cout << "sum = " << sum << endl;
   double mul_pi = accumulate(v1, v1 + 3, 1.0, multiply<double>());
   cout << "mul_pi = " << mul_pi << endl;
   mul_pi= accumulate(v1,v1+3,1, [] (const double& a , const double& b ){
           return a *b;
   }); 
   cout << "mul_pi = " <<  mul_pi << endl;
}

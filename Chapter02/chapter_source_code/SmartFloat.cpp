//---- SmartFloat.cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
class SmartFloat {
     double _value; // underlying store
   public:
      SmartFloat(double value) : _value(value) {}
      SmartFloat() : _value(0) {}
      SmartFloat( const SmartFloat& other ) { _value = other._value; }

      SmartFloat& operator = ( const SmartFloat& other ) {
          if ( this != &other ) {  _value = other._value;}
          return *this;
      }
      SmartFloat& operator = (double value ) {
          _value = value; return *this;
       }
      ~SmartFloat(){ }
      SmartFloat& operator ++ () { _value++; return *this; }
      SmartFloat operator ++ (int) { // postfix operator
		SmartFloat nu(*this); ++_value; return nu; 
      }
      SmartFloat& operator -- () { _value--; return *this; }
      SmartFloat operator -- (int) { 
          SmartFloat nu(*this);  --_value;  return nu; 
      } 
      SmartFloat& operator += ( double x ) {  _value += x; return *this;}
      SmartFloat& operator -= ( double x ) {  _value -= x;return *this; } 
      SmartFloat& operator *= ( double x ) {  _value *= x; return *this;}
      SmartFloat& operator /= ( double x ) {  _value /= x; return *this;} 

      bool operator > ( const SmartFloat& other ) {
               return _value > other._value;
      }
      bool operator < ( const SmartFloat& other ) {
               return _value < other._value;
      }
      bool operator == ( const SmartFloat& other ) {
               return _value == other._value;
      }
      bool operator != ( const SmartFloat& other ) {
               return _value != other._value;
      }
      bool operator >= ( const SmartFloat& other ) {
                return _value >= other._value;
      }
      bool operator <= ( const SmartFloat& other ) {
               return _value <= other._value;
      }
      operator int () { return _value; } 
      operator double () { return _value;}     
};

double Accumulate( double a[] , int count ){
    double value = 0;
    for( int i=0; i<count; ++i) { value += a[i]; }
    return value;
}
double Accumulate( SmartFloat a[] , int count ){
    SmartFloat value = 0;
    for( int i=0; i<count; ++i) { value += a[i]; }
    return value;
}
int main() {
   // using C++ 1z’s imitialization list 
   double x[] = { 10.0,20.0,30,40 }; 
   SmartFloat y[] = { 10,20.0,30,40 };
   double res = Accumulate(x,4); // will call the double version
   cout << res << endl;
   res = Accumulate(y,4); // will call the SmartFloat version
   cout << res << endl;   
 }

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


template <class T>
class SmartValue
{
     T _value;
   public:
      SmartValue(T value) : _value(value) {}

      SmartValue() : _value(0) {}

      SmartValue( const SmartValue& other ) {
             _value = other._value;
      }

      
      SmartValue& operator = ( const SmartValue& other ) {
                if ( this != &other )
                 {
            _value = other._value;

                 }
                 return *this;
      }

      ~SmartValue() {  }

      //////////////////////////////////////////
      //
      //
      SmartValue& operator ++ ()
      { 
           _value++; return *this; 
      }

      SmartValue operator ++ (int) 
      { 
          SmartValue nu(*this); 
          ++_value; 
          return nu; 
      }

      SmartValue& operator -- ()
      { 
           _value--; return *this; 
      }

      SmartValue operator -- (int) 
      { 
          SmartValue nu(*this); 
          --_value; 
          return nu; 
      }

      SmartValue& operator += ( T x ) 
      { 
           _value += x; return *this; 
      }

      SmartValue& operator -= ( T x ) 
      { 
         _value -= x; 
         return *this; 
      } 
  
      SmartValue& operator *= ( T x ) 
      { 
          _value *= x; return *this; 
       
      }

      SmartValue& operator /= ( T x ) 
      { 
            _value /= x; 
            return *this; 
      }   

      bool operator > ( const SmartValue& other ) {
               
        return _value > other._value;
      }

      bool operator < ( const SmartValue& other ) {
               
        return _value < other._value;
      }

      bool operator == ( const SmartValue& other ) {
               
        return _value == other._value;
      }

      bool operator != ( const SmartValue& other ) {
               
        return _value != other._value;
      }

       
      bool operator >= ( const SmartValue& other ) {
               
        return _value >= other._value;
      }

      bool operator <= ( const SmartValue& other ) {
               
        return _value <= other._value;
      }

     
 
      operator T () { return _value; }      

};

template <class T>
T Accumulate( T a[] , int count )
{
    T value = 0;

    for( int i=0; i<count; ++i) { value += a[i]; }

    return value;

}


#if 0
int main()
{

   SmartValue<int> ps(10);
   SmartValue<int> test(ps++);

   cout << (int) test << endl;

   ++ps;
   ps /=2;
   ps *=4;
   cout << (int) ( ps | SmartValue<int>(2) )  << endl;

   vector< SmartValue<int> > vs(3);

   vs[0] = SmartValue<int>(20);
   vs[1] = SmartValue<int>(10);
   vs[2] = SmartValue<int>(30);

   SmartValue<int> sum;

   for_each( vs.begin() , vs.end() , [&] ( SmartValue<int>& o ) {

           sum += o;
    }); 

   cout << (int)sum << endl;

   sort(vs.begin() , vs.end() );

   for_each( vs.begin() , vs.end() , [&] ( SmartValue<int>& o ) {

           cout << (int)o << endl;
    }); 

 }



int main()
{

   SmartValue<double> ps(10);
   SmartValue<double> test(ps++);

   cout << (int) test << endl;

   ++ps;
   ps /=2;
   ps *=4;
   cout << (int) ( ps | SmartValue<double>(2) )  << endl;

   vector< SmartValue<double> > vs(3);

   vs[0] = SmartValue<double>(20);
   vs[1] = SmartValue<double>(10);
   vs[2] = SmartValue<double>(30);

   SmartValue<double> sum;

   for_each( vs.begin() , vs.end() , [&] ( SmartValue<double>& o ) {

           sum += o;
    }); 

   cout << (int)sum << endl;

   sort(vs.begin() , vs.end() );

   for_each( vs.begin() , vs.end() , [&] ( SmartValue<double>& o ) {

           cout << (int)o << endl;
    }); 

 }

#else

int main()
{

   double x[] = { 10.0,20.0,30,40 };
   SmartValue<double> y[] = { 10,20.0,30,40 };


   double res = Accumulate(x,4);
   cout << res << endl;

   res = Accumulate(y,4);

   cout << res << endl;
 }

#endif

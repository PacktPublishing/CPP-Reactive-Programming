/////////////////
//-------- LambdaFourth.cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
//--- Generic functions  for Comparison and Swap
template <typename T>
bool Cmp( T& a , T&b ) {return ( a > b ) ? true: false;}
template <typename T>
void Swap( T& a , T&b ) { T c = a;a = b;b = c;}
template <class T>
void BubbleSortFunctor( T *arr , int length ) {
	for( int i=0; i< length-1; ++i )
		for(int j=i+1; j< length; ++j ) 
			if ( Cmp( arr[i] , arr[j] ) )
				Swap(arr[i],arr[j] );
}


template <typename T>
void BubbleSorLambda( T *arr , int length ) {
auto CmpLambda = [] (const auto& a , const auto& b ) 
{ return ( a > b ) ? true: false; };
auto SwapLambda = [] ( auto& a , auto& b ) 
{ auto c = a;a = b;b = c;};
       for( int i=0; i< length-1; ++i )
for(int j=i+1; j< length; ++j ) 
if ( CmpLambda( arr[i] , arr[j] ) )
SwapLambda (arr[i],arr[j] ); 
}


template <typename T>
void Print( const T& container){
	for(auto i = container.begin() ; i != container.end(); ++i )
			cout << *i << "\n" ;
} 

int main( int argc , char **argv ){
      double ar[4] = {20,10,15,-41};
BubbleSortFunctor(ar,4);
vector<double> a(ar,ar+4);
Print(a);
cout << "=========================================" << endl;
ar[0] = 20;ar[1] = 10;ar[2] = 15;ar[3] = -41;
BubbleSorLambda(ar,4);
vector<double> a1(ar,ar+4);
Print(a1);
cout << "=========================================" << endl;
}


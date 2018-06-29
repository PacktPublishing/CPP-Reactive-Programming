//-------------- LambdaSecond.cpp
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;
int main() {
	auto num_vect =
		vector<int>{ 10, 23, -33, 15, -7, 60, 80};
	//-- Define a BinaryOperation Lambda at the call site
	auto accum = std::accumulate(
		std::begin(num_vect), std::end(num_vect), 0, 
          [](auto const s, auto const n) {return s + n;});
         cout << accum << endl;
}

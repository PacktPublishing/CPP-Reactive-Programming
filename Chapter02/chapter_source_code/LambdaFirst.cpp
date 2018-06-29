//LambdaFirst.cpp
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
using namespace std;
int main() {
	auto num_vect =
		vector<int>{ 10, 23, -33, 15, -7, 60, 80};
	//---- Define a Lambda Function to Filter out negatives
	auto filter = [](int const value) {return value > 0; };
	auto cnt= count_if(
		begin(num_vect), end(num_vect),filter);
	cout << cnt << endl;

}

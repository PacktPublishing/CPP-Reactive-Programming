//----- AutoFirst.cpp
#include <iostream>
#include <vector>
using namespace std;
int main(){
    vector<string> vt = {"first", "second", "third", "fourth"};
    //--- Explicity specify the Type ( makes it verbose)
    for (vector<string>::iterator it = vt.begin(); it != vt.end(); it++)
        	cout << *it << " ";
    //--- Let the compiler infer the type for us
    for (auto it2 = vt.begin(); it2 != vt.end(); it2++)
       	 cout << *it2 << " ";
    return 0;
}

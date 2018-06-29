//----- AutoSecond.cpp
#include <iostream>
#include <vector>
#include <initializer_list>
using namespace std;
int main() {
    vector<double> vtdbl = {0, 3.14, 2.718, 10.00};
    auto vt_dbl2 = vtdbl; // type will be deduced
    auto size = vt_dbl2.size(); // size_t
    auto &rvec = vtdbl; // specify a auto reference
    cout << size << endl;
    // Iterate – Compiler infer the type 
    for ( auto it = vtdbl.begin(); it != vtdbl.end(); it++)
        cout << *it << " ";
    // 'it2' evaluates to iterator to vector of double
    for (auto it2 = vt_dbl2.begin(); it2 != vt_dbl2.end(); it2++)
        cout << *it2 << " ";
    // This will change the first element of vtdbl vector
    rvec[0] = 100;
    // Now Iterate to reflect the type 
    for ( auto it3 = vtdbl.begin(); it3 != vtdbl.end(); it3++)
        cout << *it3 << " ";
    return 0;
}

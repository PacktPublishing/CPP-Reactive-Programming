//----- Shared_Ptr.cpp
#include <iostream>
#include <memory>
#include <stdio.h>
using namespace std;
////////////////////////////////////////
// Even If you pass shared_ptr<T> instance
// by value, the update is visible to callee 
// as shared_ptr<T>'s copy constructor reference
// counts to the orgininal instance
// 

void foo_byvalue(std::shared_ptr<int> i) { (*i)++;}

///////////////////////////////////////
// passed by reference,we have not
// created a copy.  
//
void foo_byreference(std::shared_ptr<int>& i) { (*i)++; }
 
int main(int argc, char **argv )
{
    auto sp = std::make_shared<int>(10);
    foo_byvalue(sp);
    foo_byreference(sp);
    //--------- The output should be 12
    std::cout << *sp << std::endl;
}

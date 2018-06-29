//---- Unique_Ptr.cpp
#include <iostream>
#include <deque>
#include <memory>
using namespace std;
int main( int argc , char **argv ) {
    // Define a Smart Pointer for STL deque container...
    unique_ptr< deque<int> >  dq(new deque<int>() );
    //------ populate values , leverages -> operator
    dq->push_front(10); dq->push_front(20);
    dq->push_back(23);  dq->push_front(16);
    dq->push_back(41); 

    auto dqiter = dq->begin();
    while ( dqiter != dq->end()) 
    { cout << *dqiter << "\n"; dqiter++; }
    //------ SmartPointer will free reference
    //------ and it's dtor will be called here
    return 0;
}

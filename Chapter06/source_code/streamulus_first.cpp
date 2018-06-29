///////////////////////////
//
//  g++ -I"./Streamulus-master/src"  -ID:\PRASEED_PERSONAL\Packet_Publishing\REACTIVE_PROGRAMMING_BOOK\Book\boost_1_66_0\boost_1_66_0 streamulus_first.cpp
//
//
//




#include "streamulus.h"

#include <iostream>
using namespace std;
using namespace streamulus;

struct print 
{    
    static double temp;
    print() { }
    template<typename T>
    T operator()(const T& value) const
    { 
        print::temp += value;
        std::cout << print::temp << std::endl;
        return value;
    }
};

double print::temp = 0;

void hello_stream()
{
    using namespace streamulus;

    // Define an input stream of strings, whose name is "Input Stream"
    InputStream<double> s = NewInputStream<double>("Input Stream", true /* verbose */);
    
    // Construct a streamulus instance
    Streamulus streamulus_engine;            
    
   

    // For each element of the stream: 
    //     compute a greeting
    //     print it 
    //     emit it into the output stream
    // 
    
    
    streamulus_engine.Subscribe(Streamify<print>( s));    

    // Insert data to the input stream
    InputStreamPut<double>(s, 10);
    InputStreamPut<double>(s, 20);
    InputStreamPut<double>(s, 30);    
}


int main()
{
    hello_stream();
    return 0;
}

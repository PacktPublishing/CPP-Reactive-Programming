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

//-------  Functors for doubling/negativing and halfving values
struct twice {    
    template<typename T>
    T operator()(const T& value) const {return value*2;}
};
struct neg {    
    template<typename T>
    T operator()(const T& value) const{ return -value; }
};
struct half{    
    template<typename T>
    T operator()(const T& value) const { return 0.5*value;}
};

struct print{    
    template<typename T>
    T operator()(const T& value) const{ 
       
        std::cout << value << std::endl;
        return value;
    }
};
struct as_string 
{
    template<typename T>
    std::string operator()(const T& value) const { 
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
};

void DataFlowGraph(){
    // Define an input stream of strings, whose name is "Input Stream"
    InputStream<double> s = NewInputStream<double>("Input Stream", false /* verbose */);
    // Construct a streamulus instance
    Streamulus streamulus_engine;            
    // Define a Data Flow Graph for Stream based computation 
    Subscription<double>::type val2 = streamulus_engine.Subscribe(Streamify<neg>(Streamify<neg>(Streamify<half>(2*s))));
    Subscription<double>::type val3 = streamulus_engine.Subscribe(Streamify<twice>(val2*0.5));
    streamulus_engine.Subscribe(Streamify<print>(Streamify<as_string>(val3*2)));
    //------------------ Ingest data into the stream
    for (int i=0; i<5; i++)
        InputStreamPut(s, (double)i);
}


int main(){
    DataFlowGraph();
    return 0;
}

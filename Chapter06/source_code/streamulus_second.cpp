#include "streamulus.h"

#include <iostream>

// The print functor from example A.
struct print 
{    
    template<typename T>
    T operator()(const T& value) const
    { 
        std::cout << value << std::endl;
        return value;
    }
};

void common_subexpressions();
void common_subexpressions()
{
    using namespace streamulus;

    InputStream<std::string> s = NewInputStream<std::string>("Input Stream", true /* verbose */);
    
    // Construct a streamulus instance
    Streamulus streamulus_engine;            
    
    // Define some constants
    std::string hello("Hello ");
    std::string exc("!");

    // For each element of the stream: 
    //     compute a greeting
    //     print it 
    //     emit it into the output stream
    // 
    // the output is assigned to the 'greeting' variable,
    // which can be used in other streamulus expressions. 
    // 
    Subscription<std::string>::type greeting = 
        streamulus_engine.Subscribe(Streamify<print>(hello + s + exc));
    
    // Use the 'greeting' variable in other streamulus expressions. 
    streamulus_engine.Subscribe(Streamify<print>(std::string("I said: ") + greeting ));
    streamulus_engine.Subscribe(Streamify<print>(std::string("All together now: ") + greeting  + exc));
    
    // Insert data to the input stream
    InputStreamPut<std::string>(s, "World");
    InputStreamPut<std::string>(s, "London");
    InputStreamPut<std::string>(s, "Streamulus");    
}


int main()
{
    common_subexpressions();
    return 0;
}

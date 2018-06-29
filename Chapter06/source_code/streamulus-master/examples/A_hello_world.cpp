//
//  hello_world.cpp
//
// Streamulus Copyright (c) 2012 Irit Katriel. All rights reserved.
//
// This file is part of Streamulus.
// 
// Streamulus is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Streamulus is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Streamulus.  If not, see <http://www.gnu.org/licenses/>.
//


// *************** Example A: Hello Stream! ***************
// Below is a complete Streamulus program that receives
// as input a stream of strings and greets each one 
// of them.
//
// The output of the program is below. The Streamulus InputStream
// prints the values that arrive on the stream, and our program 
// prints the rest:
//
// -------------   Input Stream <-- World   -------------
// Hello World!
// -------------   Input Stream <-- London   -------------
// Hello London!
// -------------   Input Stream <-- Streamulus   -------------
// Hello Streamulus!
//
// ********************************************************

#include "../src/streamulus.h"

#include <iostream>

// print is a functor that prints whatever it gets. 
// Stremify<print> is a stream function that prints every
// element of a stream.
struct print 
{    
    template<typename T>
    T operator()(const T& value) const
    { 
        std::cout << value << std::endl;
        return value;
    }
};

void hello_stream();

void hello_stream()
{
    using namespace streamulus;

    // Define an input stream of strings, whose name is "Input Stream"
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
    
    
    streamulus_engine.Subscribe(Streamify<print>(hello + s + exc));    

    // Insert data to the input stream
    InputStreamPut<std::string>(s, "World");
    InputStreamPut<std::string>(s, "London");
    InputStreamPut<std::string>(s, "Streamulus");    
}

#if defined(USE_MAIN)
int main()
{
    hello_stream();
    return 0;
}
#endif

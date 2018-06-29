//
//  B_common_subexpression.cpp
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

// *********** Example B: Common Subexpression ***********
// This is an extension of Example A where the greeting
// we compute for every element of the stream is needed
// in more than one expression (outdegree > 1 in the graph).
// 
// The greeting is computed and streamed through the Stremified
// print functor as before, but we also capture a handle to 
// the output of print, which can be used in further expressions.
//
// The output of the program is:
//
// -------------   Input Stream <-- World   -------------
// Hello World!
// I said: Hello World!
// All together now: Hello World!!
// -------------   Input Stream <-- London   -------------
// Hello London!
// I said: Hello London!
// All together now: Hello London!!
// -------------   Input Stream <-- Streamulus   -------------
// Hello Streamulus!
// I said: Hello Streamulus!
// All together now: Hello Streamulus!!
// *******************************************************

#include "../src/streamulus.h"

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

#if defined(USE_MAIN)
int main()
{
    common_subexpressions();
    return 0;
}
#endif


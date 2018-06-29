//
//  D_sliding_window_function.cpp
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

#include "../src/streamulus.h"
#include "../src/sliding_window_library.h"
#include "../src/sliding_window_func.h"

#include <iostream>

// -------------------------------------------------------------------
// This example shows how to use a sliding window function.
// The WindowSum function computes the sum of the last n items of a 
// stream of doubles (we set n=3). The function itself is defined
// in sliding_window_library.h
//
// The output of the program is:
// 
// -------------   TS <-- 0   -------------
// 0
// -------------   TS <-- 1   -------------
// 1
// -------------   TS <-- 2   -------------
// 3
// -------------   TS <-- 3   -------------
// 6
// -------------   TS <-- 4   -------------
// 9
// -------------   TS <-- 0   -------------
// 7
// -------------   TS <-- 1   -------------
// 5
// -------------   TS <-- 2   -------------
// 3
// -------------   TS <-- 3   -------------
// 6
// -------------   TS <-- 4   -------------
// 9
// -------------   TS <-- 0   -------------
// 7
// -------------   TS <-- 1   -------------
// 5
// -------------   TS <-- 2   -------------
// 3
// -------------   TS <-- 3   -------------
// 6
// -------------   TS <-- 4   -------------
// 9
// -------------------------------------------------------------------

// Functor that prints whatever it gets. 
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



void sliding_window_function_example();

void sliding_window_function_example()
{
    using namespace streamulus;
    
    InputStream<double> ts = NewInputStream<double>("TS", true /* verbose */);
    Streamulus engine;
    
    boost::proto::terminal<window_>::type window = {};
    // Print the sum of the last three numbers:
    engine.Subscribe(Streamify<print>(Streamify<WindowFunc<WindowSum<double> > >(window(3,ts)) ));
        
    for (int i=0; i<15; i++)
        InputStreamPut(ts, double(i % 5));
}

#if defined(USE_MAIN)
int main()
{
    sliding_window_function_example();
    return 0;
}
#endif

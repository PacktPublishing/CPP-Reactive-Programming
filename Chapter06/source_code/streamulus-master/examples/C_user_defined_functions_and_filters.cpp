//
// C_user_defined_functions_and_filters.cpp
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

#include <iostream>

// -------------------------------------------------------------------
// This example implements an algorithm to detect crossings of two 
// moving averages of a time series.
// The input is given as {time, value} pairs.
// We have a slow-decaying moving average and a fast-decaying one.
// When they cross we issue an alert (in this case print to the 
// screen). We distinguish between Death Crosses (where the slow
// mavg crosses the fast one from below, so the time series is 
// decreasing) and Golden Crosses (where it crosses from above, 
// so the time series is increasing). 
//
// The example demonstrates expressions using user-defined functions, 
// * with and without filters
// * Streamified by type or by object.
//
// The output of the program is:
//
// -------------   TS <-- {0,0}   -------------
// Slow Mavg = 0
// Death cross detected!
// Fast Mavg = 0
// -------------   TS <-- {1,1}   -------------
// Slow Mavg = 0.632121
// Golden cross detected!
// Fast Mavg = 0.999955
// -------------   TS <-- {2,2}   -------------
// Slow Mavg = 1.49679
// Fast Mavg = 1.99995
// -------------   TS <-- {3,3}   -------------
// Slow Mavg = 2.447
// Fast Mavg = 2.99995
// -------------   TS <-- {4,4}   -------------
// Slow Mavg = 3.42868
// Fast Mavg = 3.99995
// -------------   TS <-- {5,0}   -------------
// Slow Mavg = 1.26134
// Death cross detected!
// Fast Mavg = 0.000181598
// -------------   TS <-- {6,1}   -------------
// Slow Mavg = 1.09614
// Fast Mavg = 0.999955
// -------------   TS <-- {7,2}   -------------
// Slow Mavg = 1.66749
// Golden cross detected!
// Fast Mavg = 1.99995
// -------------   TS <-- {8,3}   -------------
// Slow Mavg = 2.5098
// Fast Mavg = 2.99995
// -------------   TS <-- {9,4}   -------------
// Slow Mavg = 3.45178
// Fast Mavg = 3.99995
// -------------   TS <-- {10,0}   -------------
// Slow Mavg = 1.26984
// Death cross detected!
// Fast Mavg = 0.000181598
// -------------   TS <-- {11,1}   -------------
// Slow Mavg = 1.09927
// Fast Mavg = 0.999955
// -------------   TS <-- {12,2}   -------------
// Slow Mavg = 1.66864
// Golden cross detected!
// Fast Mavg = 1.99995
// -------------   TS <-- {13,3}   -------------
// Slow Mavg = 2.51022
// Fast Mavg = 2.99995
// -------------   TS <-- {14,4}   -------------
// Slow Mavg = 3.45194
// Fast Mavg = 3.99995
//
// --------------------------------------------------------------------

// time-value pair
struct TimeValue
{
    TimeValue()
        : time(0)
        , value(0)
    {
    }
    
    TimeValue(clock_t t, double v)
        : time(t)
        , value(v)
    {
    }
    
    friend std::ostream& operator<<(std::ostream& os, const TimeValue& tv)
    {
        return os << "{" << tv.time << "," << tv.value << "}";
    }
    
    clock_t time;
    double value;
};

// Exponentially decaying moving average
class Mavg
{
public:
    
    Mavg(int decay_factor)
    : mFirst(true)
    , mDecayFactor(decay_factor)
    , mMavg(0)
    {
    }

    double operator()(const TimeValue& tick)
    {
        if (! mFirst)
        {
            double alpha = 1-1/exp(mDecayFactor*(tick.time-mPrevTime));
            mMavg += alpha*(tick.value - mMavg);
            mPrevTime = tick.time;
        }
        else
        {
            mMavg = tick.value;
            mPrevTime = tick.time;
            mFirst = false;
        }
        return mMavg;
    }
    
private:
    clock_t mPrevTime;    
    bool mFirst;
    int mDecayFactor;
    double mMavg;  
};

// Remove consecutive repetitions from a stream. 
template<typename T>
class unique
{
public:
    
    unique()  
        : mFirst(true) 
    {
    }

    boost::optional<T> operator()(const T& value) 
    {
        if (mFirst || (value != mPrev))
        {
            mFirst = false;
            return mPrev = value;
        }
        return boost::none;
    }
        
private:
    bool mFirst; 
    T mPrev;
};


// Print an alert when a cross comes. Value indicates 
// the type of the cross.
struct cross_alert
{
    bool operator()(const bool is_golden_cross) 
    {
        if (is_golden_cross)
            std::cout << "Golden cross detected!" << std::endl;
        else
            std::cout << "Death cross detected!" << std::endl;
        
        return is_golden_cross;
    }
    
};

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


// Return whatever you got as a string. This is useful 
// for printing sub-expression results within a string
// (converting to string allows + with other strings).
struct as_string 
{
    template<typename T>
    std::string operator()(const T& value) const
    { 
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
};

void cross_alert_example();

void cross_alert_example()
{
    using namespace streamulus;
    
    InputStream<TimeValue> ts = NewInputStream<TimeValue>("TS", true /* verbose */);
    Streamulus engine;

    // The moving averages are streamified from the function objects.
    Mavg mavg1(1);
    Mavg mavg10(10);
    
    Subscription<double>::type slow = engine.Subscribe(Streamify(mavg1)(ts));
    Subscription<double>::type fast = engine.Subscribe(Streamify(mavg10)(ts));

    // print and cross_alert are streamified from the types (this work because they 
    // can be default constructed).
    
    // Print the moving averages:
    engine.Subscribe(Streamify<print>(std::string("Slow Mavg = ") + Streamify<as_string>(slow)));
    engine.Subscribe(Streamify<print>(std::string("Fast Mavg = ") + Streamify<as_string>(fast)));

    // The cross detection expression:
    engine.Subscribe(Streamify<cross_alert>( Streamify<unique<bool> >(slow < fast)));

    for (int i=0; i<15; i++)
        InputStreamPut(ts, TimeValue(i, i % 5));
}

#if defined(USE_MAIN)
int main()
{
    cross_alert_example();
    return 0;
}
#endif

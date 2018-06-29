#include "../src/streamulus.h"

#include <iostream>


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

int main()
{
    cross_alert_example();
    return 0;
}

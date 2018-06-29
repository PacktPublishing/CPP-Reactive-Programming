//====================================================================
// C++ concurrency - Thread synchronization using condition variable.
// Program to print 1-100 in order using odd and even number threads.
//====================================================================
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Global mutex
std::mutex numMutex;
std::condition_variable syncCond;
auto bEvenReady = false;
auto bOddReady  = false;

// Function to print even numbers
void printEven(int max)
{
    for (unsigned i = 0; i <= max; i +=2)
    {
        std::unique_lock<std::mutex> lk(numMutex);
        syncCond.wait(lk, []{return bEvenReady;});
        
        std::cout << i << ",";
        
        bEvenReady = false;
        bOddReady  = true;
        syncCond.notify_one();
    }
}

// Function to print odd numbers
void printOdd(int max)
{
    for (unsigned i = 1; i <= max; i +=2)
    {
        std::unique_lock<std::mutex> lk(numMutex);
        syncCond.wait(lk, []{return bOddReady;});
        
        std::cout << i << ",";
        
        bEvenReady = true;
        bOddReady  = false;
        syncCond.notify_one();
    }
}

int main()
{
    auto max = 100;
    bEvenReady = true;
    
    std::thread t1(printEven, max);
    std::thread t2(printOdd, max);
    
    if (t1.joinable())
        t1.join();
    if (t2.joinable())
        t2.join();
    
}

//====================================================================
// C++ concurrency - Program to avoid dead lock condition with mutex.
// Program to print 1-100 in order using odd and even number threads.
//====================================================================
#include <iostream>
#include <thread>
#include <mutex>

// Global mutexes
std::mutex evenMutex;
std::mutex oddMutex;

// Function to print even numbers
void printEven(int max)
{
    for (unsigned i = 0; i <= max; i +=2)
    {
        std::lock(evenMutex, oddMutex);
        std::lock_guard<std::mutex> lk_even(evenMutex, std::adopt_lock);
        std::lock_guard<std::mutex> lk_odd(oddMutex, std::adopt_lock);
        //oddMutex.lock();
        std::cout << i << ",";
        //evenMutex.lock();
        //oddMutex.unlock();
        //evenMutex.unlock();
    }
}

// Function to print odd numbers
void printOdd(int max)
{
    for (unsigned i = 1; i <= max; i +=2)
    {
        std::lock(evenMutex, oddMutex);
        std::lock_guard<std::mutex> lk_even(evenMutex, std::adopt_lock);
        std::lock_guard<std::mutex> lk_odd(oddMutex, std::adopt_lock);
        //oddMutex.lock();
        std::cout << i << ",";
        //evenMutex.lock();
        //oddMutex.unlock();
        //evenMutex.unlock();
    }
}

int main()
{
    auto max = 100;
    
    std::thread t1(printEven, max);
    std::thread t2(printOdd, max);
    
    if (t1.joinable())
        t1.join();
    if (t2.joinable())
        t2.join();
}

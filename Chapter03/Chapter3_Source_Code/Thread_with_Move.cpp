//=========================================================
// C++ concurrency - Transferring the ownership of a thread
//=========================================================

#include <iostream>
#include <thread>

void function1()
{
    std::cout << "Function 1\n";
}

void function2()
{
    std::cout << "Function 2\n";
}

int main()
{
    std::thread t1(function1);
    
    // Onwership of t1 is transferred to t2
    std::thread t2 = std::move(t1);
    
    t1 = std::thread(function2);
    
    // thread instace Created without any associated thread execution
    std::thread t3;
    
    // Onwership of t2 is transferred to t3
    t3 = std::move(t2);
    
    // No need to join t1, no longer has any associated thread of execution
    if (t1.joinable())  t1.join();
    if (t3.joinable())  t3.join();
    
    return 1;
}

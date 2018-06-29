//====================================================
// C++ concurrency - Hello world
//====================================================

#include <iostream>
#include <string>
#include <thread>

// The function to execute on the new thread.
void thread_proc(std::string msg)
{
    std::cout << "thread_proc says: " << msg;
}

int main()
{
    // Create a new thread and execute 'thread_proc' on it.
    std::thread t(thread_proc, "Hello World");
    
    // Waitng for the thread_proc to complete the execution
    // before exiting from the program
    t.join();
}

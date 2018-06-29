//====================================================
// C++ concurrency - Lambda functions into thread
//====================================================

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex m;

void print_id(int id)
{
    m.lock();
    std::cout << "Thread #" << id << std::endl;
    m.unlock();
}

int main()
{
    std::vector<std::thread> threads;
    for (int i = 1; i <= 10; ++i)
    {
        threads.push_back(std::thread(print_id, i));
    }
    
    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
    
    return 0;
}

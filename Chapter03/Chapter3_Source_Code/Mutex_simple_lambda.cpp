//====================================================
// C++ concurrency - Lambda functions into thread
//====================================================

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

std::mutex m;

int main()
{
    std::vector<std::thread> threads;
    for (int i = 1; i < 10; ++i)
    {
        threads.push_back(std::thread( [i]() {
            std::lock_guard<std::mutex> guard(m);
            //m.lock();
            std::cout << "Thread #" << i << std::endl;
            //m.unlock();
        }));
    }
    
    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
}

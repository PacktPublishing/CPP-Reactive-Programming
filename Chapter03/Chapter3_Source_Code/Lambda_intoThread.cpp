//====================================================
// C++ concurrency - Lambda functions into thread
//====================================================

#include <iostream>
#include <thread>
#include <vector>

int main()
{
    std::vector<std::thread> threads;
    for (int i = 1; i < 10; ++i)
    {
        threads.push_back(std::thread( [i]() {
            std::cout << "Thread #" << i << std::endl;
        }));
    }
    
    std::cout << "\nMain function";
    
    std::for_each(threads.begin(), threads.end(), [](std::thread &t) {
        t.join();
    });
}

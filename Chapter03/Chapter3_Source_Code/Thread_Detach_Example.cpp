//======================================================
// C++ concurrency - Example to show the usage of detach
//======================================================

#include <iostream>
#include <thread>
#include <chrono>

class parallel_job
{
    int _iterations;
    
public:
    parallel_job(int input): _iterations(input)
    {}
    
    void operator() ()
    {
        for (int i = 0; i <= _iterations; ++i)
        {
            std::cout << "iteration #" << i << std::endl;
        }
    }
};

void func()
{
    int local_val = 1000;
    parallel_job job(local_val);
    
    std::thread t(job);
    
    if (t.joinable())
        t.detach();
}

int main()
{
    func();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    return 0;
}

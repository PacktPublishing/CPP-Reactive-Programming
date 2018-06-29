//============================================================
// C++ concurrency - task-based parallelism
// Sum of elements in a vector using packaged_task with lambda
//============================================================

#include <iostream>
#include <vector>
#include <numeric>
#include <future>

int main()
{
    // Creating a packaged_task encapsulates a function
    std::packaged_task<int(std::vector<int>)> task([](std::vector<int> v) {
        return std::accumulate(v.begin(), v.end(), 0);
    });
    
    // Fetch associated future from packaged_task
    std::future<int> result = task.get_future();
    
    std::vector<int> nums{1,2,3,4,5,6,7,8,9,10};
    
    // Pass packaged_task to thread to run asynchronously
    std::thread t(std::move(task), std::move(nums));
    
    t.join();
    
    // Fetch the result of packaged_task, the value returned by calc_sum()
    int sum = result.get();
    
    std::cout << "Sum = " << sum << std::endl;
    return 0;
}
//========================================================
// C++ concurrency - task-based parallelism
// Sum of elements in an integer vector using packaged_task
//=========================================================

#include <iostream>
#include <vector>
#include <numeric>
#include <future>

// Function to calculate the sum of elements in an integer vector
int calc_sum(std::vector<int> v)
{
    int sum = std::accumulate(v.begin(), v.end(), 0);
    return sum;
}

int main()
{
    // Creating a packaged_task encapsulates a function
    std::packaged_task<int(std::vector<int>)> task(calc_sum);
    
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

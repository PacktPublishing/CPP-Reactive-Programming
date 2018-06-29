//========================================================
// C++ concurrency - task-based parallelism
// Sum of elements in a vector using async
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
    std::vector<int> nums{1,2,3,4,5,6,7,8,9,10};
    
    // Fetch associated future from packaged_task
    std::future<int> result( async(calc_sum, std::move(nums)));
    
    // Ftech the result of packaged_task, the value returned by calc_sum()
    int sum = result.get();
    
    std::cout << "Sum = " << sum << std::endl;
    return 0;
}

//=====================================================
// C++ concurrency - task-based parallelism
// Sum of elements in a vector using async with lambda
//=====================================================

#include <iostream>
#include <vector>
#include <numeric>
#include <future>

int main()
{
    std::vector<int> nums{1,2,3,4,5,6,7,8,9,10};
    
    // Fetch associated future from packaged_task
    std::future<int> result( async([](std::vector<int> v) {
        return std::accumulate(v.begin(), v.end(), 0);
    }, std::move(nums)));
    
    // Ftech the result of packaged_task, the value returned by calc_sum()
    int sum = result.get();
    
    std::cout << "Sum = " << sum << std::endl;
    return 0;
}

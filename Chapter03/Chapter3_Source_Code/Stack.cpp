//====================================================================
// C++ concurrency - stack using mutex and condition variable
//====================================================================
#include <thread>
#include <stack>
#include <mutex>
#include <condition_variable>
#include <cstdio>

template <typename T>
class Stack
{
private:
    std::stack<T> myData;
    mutable std::mutex myMutex;
    std::condition_variable myCond;
    
public:
    Stack() = default;
    ~Stack() = default;
    Stack& operator=(const Stack&) = delete;
    
    Stack(const Stack& that)
    {
        std::lock_guard<std::mutex> lock(that.myMutex);
        myData = that.myData;
    }
    
    void push(T new_val)
    {
        std::lock_guard<std::mutex> local_lock(myMutex);
        myData.push(new_val);
        myCond.notify_one();
    }
    
    bool try_pop(T& return_value)
    {
        std::lock_guard<std::mutex> local_lock(myMutex);
        if (myData.empty()) return false;
        return_value = myData.top();
        myData.pop();
        return true;
    }
    
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> local_lock(myMutex);
        if (myData.empty()) return std::shared_ptr<T>();
        
        std::shared_ptr<T> return_value(std::make_shared<T>(myData.top()));
        myData.pop();
        
        return return_value;
    }
    
    void wait_n_pop(T& return_value)
    {
        std::unique_lock<std::mutex> local_lock(myMutex);
        myCond.wait(local_lock, [this]{ return !myData.empty(); });
        return_value = myData.top();
        myData.pop();
    }
    
    std::shared_ptr<T> wait_n_pop()
    {
        std::unique_lock<std::mutex> local_lock(myMutex);
        myCond.wait(local_lock, [this]{ return !myData.empty(); });
        std::shared_ptr<T> return_value(std::make_shared<T>(myData.top()));
        return return_value;
    }
    
    bool empty()
    {
        std::lock_guard<std::mutex> local_lock(myMutex);
        return myData.empty();
    }
};

void push_test_fn(Stack<int>& s)
{
    for (int i = 0; i < 10; ++i)
    {
        printf("pushing %d\n", i);
        s.push(i);
    }
}

void pop_test_fn(Stack<int>& s)
{
    for (int i = 0; i < 10; ++i)
    {
        printf("poping %d\n", *s.wait_n_pop());
    }
}

int main()
{
    Stack<int> s;
    std::thread t1(push_test_fn,std::ref(s));
    std::thread t2(pop_test_fn,std::ref(s));
    
    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();
}

//=============================================
// C++ Atomic types explained
// A spin lock mutex using std::atomic_flag
//=============================================
#include <thread>
#include <vector>
#include <iostream>
#include <atomic>

// Class implemented spin-lock mutex
class spin_lock
{
    std::atomic_flag flg;
    
public:
    spin_lock() : flg(ATOMIC_FLAG_INIT)
    {}
    
    void lock()
    {
        // acquire lock and spin
        while (flg.test_and_set());
    }
    
    void unlock()
    {
        // release lock
        flg.clear();
    }
};

// spin_lock mutex global instance
spin_lock spin;

void func(int id)
{
    for (int count = 0; count < 10; ++count)
    {
        spin.lock();
        std::cout << "Output from thread# " << id << "  Count# " << count << '\n';
        spin.unlock();
    }
}

int main()
{
    std::vector<std::thread> v;
    for (int id = 0; id < 10; ++id) {
        v.emplace_back(func, id);
    }
    for (auto& t : v) {
        t.join();
    }
}

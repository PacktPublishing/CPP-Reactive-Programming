//================================================
// C++ Atomic operations memory ordering
// An example to demonstrate sequential consistency
//================================================
#include <atomic>
#include <iostream>
#include <string>
#include <thread>

std::string result;
std::atomic<bool> ready(false);

void thread1(){
    while(!ready.load(std::memory_order_seq_cst));
    result += "consistency";
}

void thread2(){
    result = "sequential ";
    ready = true;
}

int main(){
    std::thread t1(thread1);
    std::thread t2(thread2);
    t1.join();
    t2.join();
    
    std::cout << "Result : " << result << '\n';
}

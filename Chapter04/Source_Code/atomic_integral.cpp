//========================================================
// C++ Atomic integral types
// A simple example to demonstrate atomic operators
//=========================================================
#include <iostream>
#include <thread>
#include <atomic>

int main()
{
    std::atomic<int> value;
    
    std::cout << "Result returned from Operation: " << value.fetch_add(5) << '\n';
    std::cout << "Result after Operation: " << value << '\n';
    
    std::cout << "Result returned from Operation: " << value.fetch_sub(3) << '\n';
    std::cout << "Result after Operation: " << value << '\n';

    std::cout << "Result returned from Operation: " << value++ << '\n';
    std::cout << "Result after Operation: " << value << '\n';
    
    std::cout << "Result returned from Operation: " << ++value << '\n';
    std::cout << "Result after Operation: " << value << '\n';
    
    value += 1;
    std::cout << "Result after Operation: " << value << '\n';
    
    value -= 1;
    std::cout << "Result after Operation: " << value << '\n';
    return 0;
}

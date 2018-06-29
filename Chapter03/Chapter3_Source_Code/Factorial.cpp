//====================================================
// C++ concurrency - Lauching a thread using functors
//====================================================

#include <iostream>
#include <thread>

class Factorial
{
private:
    long double _fact;
    
public:
    Factorial() : _fact (1)
    {
    }
    
    void operator ()(int number)
    {
        _fact = 1;
        for (int i = 1; i <= number; ++i)
        {
            _fact *= i;
        }
        std::cout << "Factorial of " << number << " is " << _fact << std::endl;
    }
};

int main()
{
    Factorial fact;
    
    std::thread t1(fact, 10);
    
    t1.join();
}

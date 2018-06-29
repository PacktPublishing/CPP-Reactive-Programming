#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>

namespace rxu=rxcpp::util;


#include <array>

int main() {

    auto o1 = rxcpp::observable<>::timer(std::chrono::milliseconds(15)).map([](int) {return 1;});

	auto o2 = rxcpp::observable<>::error<int>(std::runtime_error("Error from source\n"));
	
auto o3 = rxcpp::observable<>::timer(std::chrono::milliseconds(5)).map([](int) {return 3;});
	
auto base = rxcpp::observable<>::from(o1.as_dynamic(), o2, o3);
	
auto values = base.merge();
	
values.
		subscribe(
			[](int v){printf("OnNext: %d\n", v);},
			
     [](std::exception_ptr eptr) { printf("OnError %s\n", rxu::what(eptr).c_str()); },
		
	[](){printf("OnCompleted\n");});

}
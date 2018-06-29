//---------- ReplayAll.cpp
#include <rxcpp/rx.hpp>
#include <memory>

int main(int argc, char *argv[]) {

  auto values = rxcpp::observable<>::interval(
                std::chrono::milliseconds(50), 
                rxcpp::observe_on_new_thread()).
                take(5).replay();
    // Subscribe from the beginning
    values.subscribe(
        [](long v){printf("[1] OnNext: %ld\n", v);},
        [](){printf("[1] OnCompleted\n");});
    // Start emitting
    values.connect();
    // Wait before subscribing
    rxcpp::observable<>::timer(
         std::chrono::milliseconds(125)).subscribe([&](long){
        values.as_blocking().subscribe(
            [](long v){printf("[2] OnNext: %ld\n", v);},
            [](){printf("[2] OnCompleted\n");});
    });

 //----------- Wait for Two Seconds
 rxcpp::observable<>::timer(
       std::chrono::milliseconds(2000)).
       subscribe([&](long){ });

}



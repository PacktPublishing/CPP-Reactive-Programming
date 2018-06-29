#include "rxcpp/rx.hpp"
#include "rxcpp/rx-test.hpp"
int main() {
auto ints = rxcpp::observable<>::create<int>( [](rxcpp::subscriber<int> s){
s.on_next(1);
s.on_next(4);
s.on_next(9);
s.on_completed();
});
ints.subscribe( [](int v){printf("OnNext: %d\n", v);},
[](){printf("OnCompleted\n");});
}
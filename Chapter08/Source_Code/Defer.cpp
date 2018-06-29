#include <rxcpp/rx.hpp>
#include <memory>

int main(int argc, char *argv[]) {

    printf("//! [defer sample]\n");
    
    auto observable_factory = [](){
      printf("Inside the factory \n");
      return rxcpp::observable<>::range(1, 3);};
 
    auto values = rxcpp::observable<>::defer(observable_factory);
    

 //   auto shared_values = values.publish();

    values.
subscribe(
           
        [](int v){printf("#1OnNext: %d\n", v);},
           
        [](){printf("#1OnCompleted\n");});
    

    values.
subscribe(
           
        [](int v){printf("#2OnNext: %d\n", v);},
           
        [](){printf("#2OnCompleted\n");});
 

    //shared_values.connect();

    printf("//! [defer sample]\n");

}


#include <rxcpp/rx.hpp>
#include <memory>

int main(int argc, char *argv[]) {

     printf("//! [scope sample]\n");
  
     typedef rxcpp::resource<std::vector<int>> resource;
    
     auto resource_factory = [](){
       printf("Resource #1\n");
       return resource(rxcpp::util::to_vector({1, 2, 3, 4, 5}));
     };
    

     auto observable_factory = [](resource res){
        printf("Resource #2\n");
        return rxcpp::observable<>::iterate(res.get());
     };
    

     auto values = rxcpp::observable<>::scope(resource_factory, 
         observable_factory);
  

     auto mpvalues = values.map( [] (int v ) { return v*v; }); 

     mpvalues.
subscribe(
         
           [](int v){printf("OnNext: %d\n", v);},
    
           [](){printf("OnCompleted\n");});
    

     mpvalues.
subscribe(
         
           [](int v){printf("#2OnNext: %d\n", v);},
    
           [](){printf("#2OnCompleted\n");});
    

     printf("//! [scope sample]\n");

}


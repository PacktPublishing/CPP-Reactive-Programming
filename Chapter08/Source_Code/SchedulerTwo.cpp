
//----------- SchedulerTwo.cpp
#include "rxcpp/rx.hpp"

int main()
{
   
    auto coordination = rxcpp::identity_current_thread();
    
    auto worker = coordination.create_coordinator().get_worker();
    auto start = coordination.now() + std::chrono::milliseconds(1);
    auto period = std::chrono::milliseconds(1);
    auto values = rxcpp::observable<>::interval(start,period).
        
            take(5).
replay(2, coordination);

  

    worker.schedule([&](const rxcpp::schedulers::schedulable&){
      
        values.subscribe(
            
               [](long v){
                   printf("#1 -- %d : %ld\n", 
                   std::this_thread::get_id(),v); 
               },
       
               [](){
                  printf("#1 --- OnCompleted\n");});
 
               });

 
   
     worker.schedule([&](const rxcpp::schedulers::schedulable&){
      
        values.subscribe(
            
               [](long v){
                   printf("#2 -- %d : %ld\n", 
                   std::this_thread::get_id(),v); 
               },
       
               [](){
                  printf("#2 --- OnCompleted\n");});
 
               });

 
    

   // Start emitting
   
   worker.schedule([&](const rxcpp::schedulers::schedulable&){
   
        values.connect();
    
   });

    
   // Add blocking subscription to see results
    
   values.as_blocking().subscribe();

   return 0;
}
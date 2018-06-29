//---------- SubscribeOn.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
#include <thread>
#include <mutex>
 
std::mutex console_mutex;

void CTDetails(int val = 0 ) {
  
   console_mutex.lock();
   std::cout << "Current Thread id => " 
           << std::this_thread::get_id() 
           << val
           << std::endl; 
   console_mutex.unlock(); 

}

void Yield( bool y ) {
   if (y) { std::this_thread::yield(); }

}

int main(){

   //----------- coordination object
   auto coordination = rxcpp::serialize_new_thread();
 
 
   //----------------- retrieve the worker

   auto worker = coordination.create_coordinator().get_worker();
   

   //-------------- Create an Obsrevable
   auto values = rxcpp::observable<>::interval(std::chrono::milliseconds(50)).
        
                 take(5).
        
                 replay(coordination);

    

   // Subscribe from the beginning
    
   worker.schedule([&](const rxcpp::schedulers::schedulable&)
   {
      
         values.subscribe(
            [](long v){CTDetails(v);},
   
                        [](){ CTDetails();});
    });

    

   // Wait before subscribing
    

   worker.schedule(coordination.now() + std::chrono::milliseconds(125),
                    [&](const rxcpp::schedulers::schedulable&){
        
                                      
                              values.subscribe(
            [](long v){CTDetails(v*v);},
   
                        [](){ CTDetails();});

    });

   

   // Start emitting
    
   worker.schedule([&](const rxcpp::schedulers::schedulable&){
      
            values.connect();
   
   });


   // Add blocking subscription to see results
    
   values.as_blocking().subscribe();
}

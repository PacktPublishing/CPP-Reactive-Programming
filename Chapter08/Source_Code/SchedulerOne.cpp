//------------- SchedulerOne.cpp

#include "rxcpp/rx.hpp"

int main()
{
    //---------- Get a Coordination 
    auto coordination = rxcpp::serialize_new_thread();
 
    //------- Create a Worker instance   
    auto worker = coordination.create_coordinator().get_worker();

    auto sub_action = rxcpp::schedulers::make_action([] 
         (const rxcpp::schedulers::schedulable&) 
         {  printf("Action Executed in Thread # : %d\n", 
                   std::this_thread::get_id());  } ); 

    auto scheduled = rxcpp::schedulers::make_schedulable(worker,sub_action);
    scheduled.schedule();
    


    return 0;
}
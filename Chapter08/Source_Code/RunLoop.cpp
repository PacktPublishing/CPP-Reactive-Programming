//------------- RunLoop.cpp
#include "rxcpp/rx.hpp"
int main()
{
   
    //------------ Print the Main Thread Id
    printf("Main Thread Id is %d\n", 
                std::this_thread::get_id());
    
    //------- Instantiate a run_loop object
    //------- which will loop in the main thread
    rxcpp::schedulers::run_loop rlp;

    //------ Create a coordination for run loop
    auto main_thread = rxcpp::observe_on_run_loop(rlp);
    auto worker_thread = rxcpp::synchronize_new_thread();

    rxcpp::composite_subscription scr;

    rxcpp::observable<>::range(0,15).
        map([](int i){
            //----- This will get executed in worker
            printf("Map %d : %d\n", std::this_thread::get_id(),i); 
            return i;
        }).
        take(5).subscribe_on(worker_thread).
        observe_on(main_thread).
        subscribe(scr, [&](int i){
            //--- This will get executed in main thread
            printf("Sub %d : %d\n", std::this_thread::get_id(),i); 
        });

    //------------ Execute the Run Loop
    while (scr.is_subscribed() || !rlp.empty()) {
        while (!rlp.empty() && rlp.peek().when < rlp.now()) {
            rlp.dispatch();
        }
    }
    return 0;
}
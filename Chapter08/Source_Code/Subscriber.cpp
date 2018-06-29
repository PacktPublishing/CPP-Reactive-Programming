//---- Subscriber.cpp


#include "rxcpp/rx.hpp"

int main()
{
     //----- create a subscription object
     auto subscription = rxcpp::composite_subscription();
     //----- Create a Subscription 
     auto subscriber = rxcpp::make_subscriber<int>(
        subscription,
        [&](int v){
            printf("OnNext: --%d\n", v);
            if (v == 3)
                subscription.unsubscribe();
        },
        [](){ printf("OnCompleted\n");});

    rxcpp::observable<>::create<int>(
        [](rxcpp::subscriber<int> s){
            for (int i = 0; i < 5; ++i) {
                if (!s.is_subscribed()) 
                    break;
                s.on_next(i);
               
            }
            s.on_completed();  
    }).subscribe(subscriber);

   return 0;
}
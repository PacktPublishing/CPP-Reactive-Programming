//----------- OperatorLiftSecond.cpp
#include "rxcpp/rx.hpp"

#include "rxcpp/rx-test.hpp"
#include <iostream>
namespace rxu=rxcpp::util;

#include <array>

using namespace rxcpp;
using namespace rxcpp::operators;

/////////////////////////////////////////////////
// The LiftAction class converts an Action ( a lambda )
// and wraps it into a form which can help us to connect
// to an observable<T> using the observable<T>::lift<T> method. 
template<class Action>
struct LiftAction
{
    typedef typename std::decay<Action>::type 
            action_type;
    action_type action;

    LiftAction(action_type t): action(t){}

    //////////////////////////////////////
    // Create an Internal observer to gather 
    // data from observable<T> 
    //
    template<class Subscriber>
    struct action_observer : public 
              rxcpp::observer_base<typename 
              std::decay<Subscriber>::type::value_type>
    {
        /////////////////////////////////////////////
        // typedefs for 
        //        * this_type (action_observer)
        //        * base_type (observable_base) 
        //        * value_type 
        //        * dest_type
        //        * observer_type
        typedef action_observer<Subscriber> this_type;
        typedef rxcpp::observer_base<typename 		
                std::decay<Subscriber>::type::value_type> base_type;
        typedef typename base_type::value_type value_type;
        typedef typename std::decay<Subscriber>::type dest_type;
        typedef rxcpp::observer<value_type, this_type> observer_type;

        //------ destination subscriber and action
        dest_type dest;
        action_type action;
        action_observer(dest_type d, action_type t)
            : dest(d), action(t){}

        //--------- subscriber/observer methods
        //--------  on_next implementation needs more 
        //--------- robustness by supporting exception handling
        void on_next(typename dest_type::value_type v) const 
        {dest.on_next(action(v));}
        void on_error(std::exception_ptr e) const 
        { dest.on_error(e);}
        void on_completed() const {
            dest.on_completed();
        }
        //--------- Create a subscriber with requisite parameter
        //--------- types
        static rxcpp::subscriber<value_type, observer_type> 
                 make(const dest_type& d, const action_type& t) {
            return rxcpp::make_subscriber<value_type>
                 (d, observer_type(this_type(d, t)));
        }
    };

    template<class Subscriber>
    auto operator()(const Subscriber& dest) const
        -> decltype(action_observer<Subscriber>::make(dest, action)) {
        return      action_observer<Subscriber>::make(dest, action);
    }
};

//////////////////////////////////////
// liftaction takes a Universal reference 
// and uses perfect forwarding 
//
template<class Action>

auto liftaction(Action&& p)
 -> 
      LiftAction<typename std::decay<Action>::type>
{
    
    return  LiftAction<typename 
          std::decay<Action>::type>(std::forward<Action>(p));

}

int main() {
     std::array< int ,4 > a={{10, 20,30,40}};
     auto h = [] (observable<int> src ) {
         return src.map([](int n ) { return n; });
     };
     auto g = [] (observable<int> src ) {
         return src.map([](int n ) { return n*2; });
     };
     // type of values will be observable<int>
     // Lazy Evaluation 
     auto values = g(h(rxcpp::observable<>::iterate(a)))
       .lift<int> (liftaction( [] ( int r ) { return 2*r; })); 
     //-------- As usual subscribe 
     values.subscribe( 
              [] (int f) { std::cout << f <<  std::endl; } , 
              [] () {std::cout << "Hello World.." << std::endl;} );
}
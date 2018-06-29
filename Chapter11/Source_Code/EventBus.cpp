//----------- EventBus.cpp
#include <rxcpp/rx.hpp>
#include <memory>
#include <map>
#include <algorithm>
using namespace std;

//---------- Event Information
struct EVENT_INFO{
   int id;
   int err_code;
   string description;
   EVENT_INFO() { id = err_code = 0 ; description ="default";}
   EVENT_INFO(int pid,int perr_code,string pdescription )
   { id = pid; err_code = perr_code; description = pdescription; }
   void Print() {
      cout << "id & Error Code" << id << ":" << err_code << ":";
      cout << description << endl;
   }
};

//----------- The following method
//----------- will be invoked by 
//----------- Consumers
template <class T>
void DoSomeThingWithEvent( T ev )
{ev.Print();}

//---------- Forward Declarations 
template <class T>
class EventBus;
//------------- Event Producer
//------------- Just Inserts event to a Bus
template <class T>
class Producer {
  string name;
 public:
   Producer(string pname ) { name = pname;}
   bool Fire(T ev,EventBus<T> *bev ) {
         bev->FireEvent(ev);
         return false;
   }
};
//------------ Event Consumer
//------------ Subscribes to a Subject
//------------ to Retrieve Events
template <class T>
class Consumer {
  string name;
  //--------- The subscription member helps us to
  //--------- Unsubscribe to an Observable 
  rxcpp::composite_subscription subscription;
public:
  Consumer(string pname) { name = pname;}
  //--------- Connect a Consumer to a Event Bus
  bool Connect( EventBus<T> *bus ) {
      //------ If already subscribed, Unsubscribe!
      if ( subscription.is_subscribed() )
             subscription.unsubscribe();
      //------- Create a new Subscription
      //------- We will call DoSomeThingWithEvent method
      //------- from lambda function
      subscription = rxcpp::composite_subscription();
      auto subscriber = rxcpp::make_subscriber<T>(
        subscription,[=](T value){
            DoSomeThingWithEvent<T>(value);
        },[](){ printf("OnCompleted\n");});
      //----------- Subscribe!
      bus->GetObservable().subscribe(subscriber);
      return true;
  }
  //-------- DTOR ....Unsubscribe
  ~Consumer() { Disconnect(); }
  bool Disconnect() {
    if (subscription.is_subscribed() )
        subscription.unsubscribe();
  }
};
//--- The implementation of the EventBus class
//--- We have not taken care of Concurrency issues
//--- as our purpose is to demonstrate the pattern
template <class T>
class EventBus
{
  private:
    std::string name;
    //----- Reference to the Subject...
    //----- Consumers get notification by 
    //----- Subscribing to the Observable side of the subject
    rxcpp::subjects::behavior<T> *replaysubject; 
  public:
    EventBus<T>() {replaysubject = new rxcpp::subjects::behavior<T>(T());}
    ~EventBus() {delete replaysubject;}
    //------ Add a Consumer to the Bus...
    bool AddConsumer( Consumer<T>& b ) {b.Connect(this);}
    //------ Fire the Event...
    bool FireEvent ( T& event ) {
       replaysubject->get_subscriber().on_next(event);
       return true;
    }
    string get_name() { return name;}
    rxcpp::observable<T> GetObservable() 
    { return replaysubject->get_observable(); }
};
/////////////////////
//The EntryPoint
//
//
int main(int argc, char *argv[]) {
    //---- Create an instance of the EventBus
    EventBus<EVENT_INFO> program_bus;
    //---- Create a Producer and Two Consumers
    //---- Add Consumers to the EventBus
    Producer<EVENT_INFO> producer_one("first");
    Consumer<EVENT_INFO> consumer_one("one");
    Consumer<EVENT_INFO> consumer_two("two");
    program_bus.AddConsumer(consumer_one);
    program_bus.AddConsumer(consumer_two);
    //---- Fire an Event...
    EVENT_INFO ev;
    ev.id = 100;
    ev.err_code = 0;
    ev.description = "Hello World..";
    producer_one.Fire(ev,&program_bus);
    //---- fire another by creating a second 
    //---- Producer
    ev.id = 100;
    ev.err_code = 10;
    ev.description = "Error Happened..";
    Producer<EVENT_INFO> producer_two("second");
    producer_two.Fire(ev,&program_bus);
}


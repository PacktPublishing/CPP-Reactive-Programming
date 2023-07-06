/////////////////////////
// Observer.cpp
// g++ -std=c++1z Observer.cpp
// 
#include <iostream>
#include  <vector>
#include <memory>
using namespace std;
//////////////////////////
// Forwad declaration of Event Sink
//
template<class T>
class EventSourceValueObserver;

/////////////////////////////
// A toy implementation of Event Source
//
template<class T>
class EventSourceValueSubject{
   vector<EventSourceValueObserver<T> *> sinks; 
   T State; // T is expected to be a value type
  public:
   EventSourceValueSubject() { State = 0; }
   ~EventSourceValueSubject() {
       for( auto *n : sinks ) { delete n; }
       sinks.clear();
   }
   bool Subscribe( EventSourceValueObserver<T> *sink )
   { sinks.push_back(sink); return true;}

   void NotifyAll() {
      for (auto sink : sinks) { sink->Update(State); }
   }
   T GetState() { return State; }
   void SetState(T pstate) { State = pstate; NotifyAll(); }
};

////////////////////////////////////
//
// An event sink class for the Event Source above

template <class T>
class EventSourceValueObserver{
    T OldState;
  public:
    EventSourceValueObserver() { OldState = 0; }
    virtual ~EventSorceValueObserver() {}
    virtual void Update( T State ) {
       cout << "Old State " << OldState << endl;
       OldState = State;
       cout << "Current State " << State << endl; 
    }
};
/////////////////////////////////
// A simple specialized Observer
//
//
class AnotherObserver : public EventSourceValueObserver<double>
{
  public:
    AnotherObserver():EventSourceValueObserver() {}
    virtual ~AnotherObserver() {}
    virtual void Update( double State ) {
      cout << " Specialized Observer" << endl; 
    }
};

///////////////////////////////////////////
// An EntryPoint 
//
int main() {

   unique_ptr<EventSourceValueSubject<double>> evsrc(new EventSourceValueSubject<double>());
   //---- Create Two instance of Observer and Subscribe 
   evsrc->Subscribe( new AnotherObserver() );
   evsrc->Subscribe( new EventSourceValueObserver<double>());

   //------ Change the State of the Event Source
   //------ This should trigger call to Upate of the Sink
   evsrc->SetState(100);

}

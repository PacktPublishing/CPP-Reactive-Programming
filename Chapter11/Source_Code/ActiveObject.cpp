#include <rxcpp/rx.hpp>
#include <memory>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <windows.h>
#include <functional>
#include <thread>
#include <future>
using namespace std;

//
//------- Active Object Pattern Implementation
//
template <class T>
class ActiveObject {
    //----------- Dispatcher Object
    rxcpp::subjects::subject<T> subj;
    protected:
    ActiveObject(){
       subj.get_observable().subscribe([=] (T s )
       {
          Execute(s);  

       });

    } 
    virtual void Execute(T s) {
        
    }
    public:
    // fire-and-forget
    void FireNForget(T item){ subj.get_subscriber().on_next(item);}
    rxcpp::observable<T> GetObservable() { return subj.get_observable(); }
   
    ~ActiveObject() {}
};

class ConcreteObject : public ActiveObject<double> {
  
   public:
     ConcreteObject() {}
     virtual void Execute(double a ) {

         cout << "Hello World....." << a << endl;
     }

};

int main(int argc, char *argv[]) {
  ConcreteObject temp;
  for( int i=0; i<=10; ++i )
      temp.FireNForget(i*i);
  return 0;
}


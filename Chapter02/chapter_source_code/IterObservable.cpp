// Iterobservable.cpp
//  we can use Range Based For loop as given below (see the main below)
//    for (suto l : EnumerableRange<5, 25>()) { std::cout << l << ' '; }       
//     std::cout << endl;
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>
using namespace std;
template<long START, long END>
class EnumerableRange {
public:
	class iterable : public std::iterator<
		std::input_iterator_tag,   // category
		long,                      // value_type
		long,                      // difference_type
		const long*,               // pointer type
		long>                      // reference type
	                       
        {
		long current_num = START;
	  public:
		reference operator*() const { return current_num; }
		explicit iterable(long val = 0) : current_num(val) {}
		iterable& operator++() { 
		       current_num = ( END >= START) ? current_num + 1 :
				 current_num - 1; 
			return *this; 
             }
		iterable operator++(int) { 
                    iterable retval = *this; ++(*this); return retval; 
             }
		bool operator==(iterable other) const 
                    { return current_num == other.current_num; }
		bool operator!=(iterable other) const 
                  	 { return !(*this == other); }
		
	};

       iterable begin() { return iterable(START); }
	iterable end() { return iterable(END >= START ? END + 1 : 
END - 1); }
};

struct  OBSERVER {
	 std::function<void(const long&)> ondata;
	 std::function<void()> oncompleted;
	 std::function<void(const std::exception &)> onexception;
};


template<long START, long END>
class ObservableRange {
private:
       //---------- Container to store observers
	std::vector<
		std::pair<const OBSERVER&,int>> _observers;
	int _id = 0;


	class iterable : public std::iterator<
		std::input_iterator_tag,   // category
		long,                      // value_type
		long,                      // difference_type
		const long*,               // pointer type
		long>                      // reference type
	                       
        {
		long current_num = START;
	public:
		reference operator*() const { return current_num; }
		explicit iterable(long val = 0) : current_num(val) {}
		iterable& operator++() { 
		
                        current_num = ( END >= START) ? current_num + 1 :
				 current_num - 1; 
			return *this; 
                }
		iterable operator++(int) { 
                    iterable retval = *this; ++(*this); return retval; 
                }
		bool operator==(iterable other) const 
                    { return current_num == other.current_num; }
		bool operator!=(iterable other) const 
                   { return !(*this == other); }
		
	};
	iterable begin() { return iterable(START); }
	iterable end() { return iterable(END >= START ? END + 1 : END - 1); }
//  generate values between the range  
//  This is a private method and will be invoked from the generate
//  ideally speaking, we should invoke this method with std::asnyc
void generate_async() 
{

  auto& subscribers = _observers;
  for( auto l : *this )
     for (const auto& obs : subscribers) {
		const OBSERVER& ob =  obs.first;
		 ob.ondata(l);
     }  
 }

 //----- The public interface of the call include generate which triggers 
 //----- the generation of the sequence, subscribe/unsubscribe pair 
 public:
        //-------- the public interface to trigger generation
        //-------- of thevalues. The generate_async can be executed
        //--------- via std::async to return to the caller
        void generate() {  generate_async();  }
        //---------- subscribe method. The clients which 
        //----------- expects notification can register here
        virtual int subscribe(const OBSERVER& call) {
			_observers.emplace_back(call, ++_id);
			return _id;
        }
	 //------------ has just stubbed unsubscribe to keep
        //------------- the listing small
        virtual void unsubscribe(const  int subscription) {}
};

int main() {
	//------ Call the Range based enumerable 
	for (long l : EnumerableRange<5, 25>()) 
       { std::cout << l << ' '; }
	std::cout << endl;

       // instantiate an instance of ObservableRange
       auto j =  ObservableRange<10,20>();
       OBSERVER test_handler;
	test_handler.ondata = [=](const long & r) 
       {cout << r << endl; };
       //---- subscribe to the notifiactions
       int cnt = j.subscribe(test_handler);
       j.generate(); //trigget events to generate notifications
       return 0;
}


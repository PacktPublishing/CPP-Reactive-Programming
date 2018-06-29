//------------------CellPattern.cpp
#include <rxcpp/rx.hpp>
#include <memory>
#include <map>
#include <algorithm>
using namespace std;
class Cell
{
  private:
    std::string name;
    std::map<std::string,Cell *> parents;
    rxcpp::subjects::behavior<double> *behsubject;  
  public:
    string get_name() { return name;}
    void SetValue(double v ) 
    { behsubject->get_subscriber().on_next(v);}
    double GetValue() 
    { return behsubject->get_value(); }
    rxcpp::observable<double> GetObservable() 
    { return behsubject->get_observable(); }
    Cell(std::string pname) {
       name = pname;
       behsubject = new rxcpp::subjects::behavior<double>(0);
    }
    ~Cell() {delete behsubject; parents.clear();}
    bool GetCellNames( string& a , string& b )
    {
         if ( parents.size() !=2 ) { return false; }
         int i = 0;
         for(auto p  : parents ) {
            ( i == 0 )? a = p.first : b = p.first;
            i++;     
         }
         return true;
    }
    /////////////////////////////
    // We will just add two parent cells...
    // in real life, we need to implement an 
    // expression evaluator
    bool Recalculate() {

        string as , bs ;
        if (!GetCellNames(as,bs) ) { return false; }
        auto a = parents[as];
        auto b = parents[bs];
        SetValue( a->GetValue() + b->GetValue() );
        return true;
    }
    bool Attach( Cell& s ) {

       if ( parents.size() >= 2 ) { return false; }
       parents.insert(
          pair<std::string,Cell *>(s.get_name(),&s));
       s.GetObservable().subscribe( [=] (double a ) {
               Recalculate() ;
                
       });
       return true;
       
    }
    
    bool Detach( Cell& s ) { //--- Not Implemented 
    }
   
};



int main(int argc, char *argv[]) {   

    Cell a("a");
    Cell b("b");
    Cell c("c");
    Cell d("d");
    Cell e("e");
    c.Attach(a);
    c.Attach(b);
  
    e.Attach(c);
    e.Attach(d);
    a.SetValue(100);  // should print 100
    cout << "Value is " << c.GetValue() << endl;
    b.SetValue(200);  // should print 300
    
    cout << "Value is " << c.GetValue() << endl;
    b.SetValue(300);  // should print 400
    
    cout << "Value is " << c.GetValue() << endl;
    d.SetValue(-400); // should be Zero
    
    cout << "Value is " << e.GetValue() << endl;
   

}


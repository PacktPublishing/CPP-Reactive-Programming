//------- BookExprEval.cpp
// g++ -std=c++1z BookExprEval.cpp
#include <iostream>
#include <memory>
#include <list>
#include <stack>
#include <functional>
#include <thread>
#include <future>
#include <random>
#include "FuncCompose.h" // available int the code base
using namespace std;
//////////////////////////////////
// List of operators supported by 
// the evaluator -  meaning of name is obvious from names
enum class OPERATOR{ ILLEGAL,PLUS,MINUS,MUL,DIV,UNARY_PLUS,UNARY_MINUS };

//------------------ Section Two

//------------ The following forward declarations
//----------- are for three kinds of nodes in the hierarchy
class Number;  //----- Stores IEEE double precision floating point number 
class BinaryExpr; //--- Node for Binary Expression
class UnaryExpr;  //--- Node for Unary Expression
class IExprVisitor; //---- Interface for the Visitor 
//---- Every node in the expression tree will inherit from
//----- The Expr class
class Expr {
  public:
   //---- The standard Visitor double dispatch method
   virtual double accept(IExprVisitor& expr_vis) = 0;
   virtual ~Expr() {}
};
////////////////////////////////
// The Visitor interface contains methods for each of the concerte node 
struct IExprVisitor{
   virtual  double Visit(Number& num) = 0;
   virtual  double Visit(BinaryExpr& bin) = 0;
   virtual  double Visit(UnaryExpr& un)=0 ;
};

//-------------------- Section Three

//---------A class to represent IEEE 754 interface
class Number : public Expr {
   double NUM;
  public:
   double getNUM() { return NUM;}   
   void setNUM(double num)   { NUM = num; }
   Number(double n) { this->NUM = n; }
   ~Number() {}
   double accept(IExprVisitor& expr_vis){ return expr_vis.Visit(*this);}
};

//----------------- Section Four

//-------------- Modeling Binary Expresison 
class BinaryExpr : public Expr {
   Expr* left; Expr* right; OPERATOR OP;
  public:
   BinaryExpr(Expr* l,Expr* r , OPERATOR op ) { left = l; right = r; OP = op;}
   OPERATOR getOP() { return OP; }
   Expr& getLeft() { return *left; }
   Expr& getRight() { return *right; }
   ~BinaryExpr() { delete left; delete right;left =0; right=0; }
   double accept(IExprVisitor& expr_vis) { return expr_vis.Visit(*this);}
};

//---------------- Section Five
//-----------------Modelling Unary Expression
class UnaryExpr : public Expr {
   Expr * right; OPERATOR op;
  public:
   UnaryExpr( Expr *operand , OPERATOR op ) { right = operand;this-> op = op;}
   Expr& getRight( ) { return *right; }
   OPERATOR getOP() { return op; }
   virtual ~UnaryExpr() { delete right; right = 0; }
   double accept(IExprVisitor& expr_vis){ return expr_vis.Visit(*this);}
};

//--------An Evaluator for Expression Composite using Visitor Pattern 
class TreeEvaluatorVisitor : public IExprVisitor{
  public:
   double Visit(Number& num){ return num.getNUM();}
   double Visit(BinaryExpr& bin) {
     OPERATOR temp = bin.getOP(); double lval = bin.getLeft().accept(*this);
     double rval = bin.getRight().accept(*this);
     return (temp == OPERATOR::PLUS) ? lval + rval: (temp == OPERATOR::MUL) ? 
         lval*rval : (temp == OPERATOR::DIV)? lval/rval : lval-rval;  
   }
   double Visit(UnaryExpr& un) {
     OPERATOR temp = un.getOP(); double rval = un.getRight().accept(*this);
     return (temp == OPERATOR::UNARY_PLUS)  ? +rval : -rval;
   }
};
//------------A Visitor to Print Expression in RPN 
class ReversePolishEvaluator : public IExprVisitor {
  public:
   double Visit(Number& num){cout << num.getNUM() << "  " << endl; return 42;}
   double Visit(BinaryExpr& bin){
     bin.getLeft().accept(*this); bin.getRight().accept(*this);
     OPERATOR temp = bin.getOP();
     cout << (  (temp==OPERATOR::PLUS) ? " + " :(temp==OPERATOR::MUL) ?
              " * " : (temp == OPERATOR::DIV) ? " / ": "  - " ) ; return 42;
   }
   double Visit(UnaryExpr& un){
     OPERATOR temp = un.getOP();un.getRight().accept(*this);
     cout << (temp == OPERATOR::UNARY_PLUS) ?" (+) " : " (-) "; return 42;
   }
};
 
//-------------------------- Listing for the Part Two
////////////////////////////
// A enum to store discriminator -> Operator or a Value?
enum class ExprKind{
      ILLEGAL_EXP,  OPERATOR , VALUE
};
//////////////////////////////////////////
// A Data structure to store the Expression node.
// A node will either be a Operator or Value
//
struct EXPR_ITEM {
    ExprKind knd;
    double Value;
    OPERATOR op;
    EXPR_ITEM():op(OPERATOR::ILLEGAL),Value(0),knd(ExprKind::ILLEGAL_EXP){}
    bool SetOperator( OPERATOR op ){
      this->op = op;this->knd = ExprKind::OPERATOR; return true;
    }
    bool SetValue(double value) {
      this->knd = ExprKind::VALUE;this->Value = value;return true;
    }
    string toString() {DumpContents();return "";}

   private:
      void DumpContents() {
                if (this->knd == ExprKind::VALUE)
                    cout << Value << " " ;
                else
                {
                    switch(this->op) {
                        case OPERATOR::PLUS: 
                            
                            cout << "+"  ;
                            break;
                        case OPERATOR::MINUS:
                            cout << "-";
                            break;
                        case OPERATOR::DIV:
                            cout << "/";
                            break;
                        case OPERATOR::MUL:
                            cout << "*";
                            break;
                        case OPERATOR::UNARY_PLUS:
                            cout << "(+)";
                            break;
                        case OPERATOR::UNARY_MINUS:
			    cout << "(-)" ;
			    break;

                            

                    }
                 }
              }

    
};

//---- A Flattener for Expressions
class FlattenVisitor : public IExprVisitor {
        list<EXPR_ITEM>  ils;
        EXPR_ITEM MakeListItem(double num)
        { EXPR_ITEM temp; temp.SetValue(num); return temp; }
        EXPR_ITEM MakeListItem(OPERATOR op)
        { EXPR_ITEM temp;temp.SetOperator(op); return temp;}
        public:
        list<EXPR_ITEM> FlattenedExpr(){ return ils;}
        FlattenVisitor(){}
        double Visit(Number& num){
           ils.push_back(MakeListItem(num.getNUM()));return 42;
        }
        double Visit(BinaryExpr& bin) {
            bin.getLeft().accept(*this);bin.getRight().accept(*this);
            ils.push_back(MakeListItem(bin.getOP()));return 42;
        }
        double Visit(UnaryExpr& un){
            un.getRight().accept(*this);
            ils.push_back(MakeListItem(un.getOP())); return 42;
        }
};

//--- A Global Function to Flatten an Expr Tree
list<EXPR_ITEM> ExprList(Expr* r) {
   unique_ptr<FlattenVisitor> fl(new FlattenVisitor());
   r->accept(*fl); list<EXPR_ITEM> ret = fl->FlattenedExpr(); 
   cout << "Size of element " << ret.size() << endl; return ret;
}

/////////////////////////////////////////////////////
//-------- A minimal stack to evaluate RPN expression
class DoubleStack : public stack<double> {
   public:
    DoubleStack() {  }
    void Push( double a ) { this->push(a);}
    double Pop() { double a = this->top(); this->pop(); return a; }
};

//------ A Global Function to Evaluate an expression
double Evaluate( list<EXPR_ITEM> ls) {
   DoubleStack stk; double n;
   for( EXPR_ITEM s : ls ) {
     if (s.knd == ExprKind::VALUE) { stk.Push(s.Value); }
     else if ( s.op == OPERATOR::PLUS) { stk.Push(stk.Pop() + stk.Pop());}
     else if (s.op == OPERATOR::MINUS ) { stk.Push(stk.Pop() - stk.Pop());}
     else if ( s.op ==  OPERATOR::DIV) { n = stk.Pop(); stk.Push(stk.Pop() / n);}
     else if (s.op == OPERATOR::MUL) { stk.Push(stk.Pop() * stk.Pop()); }
     else if ( s.op == OPERATOR::UNARY_MINUS) { stk.Push(-stk.Pop()); }

   }
   double d = stk.Pop();
   return d;
}

//-----  Global Function Evaluate an Expression Tree
double Evaluate( Expr* r ) { return Evaluate(ExprList(r)); }

//------------------ Global Function to Iterate through the list 
void Iterate( list<EXPR_ITEM>& s ){
    for (auto n : s ) { std::cout << n.toString()  << '\n';}
}

//----------------- OBSERVER interface
struct  OBSERVER {
	 int id;
	 std::function<void(const double)> ondata;
	 std::function<void()> oncompleted;
	 std::function<void(const std::exception &)> onexception;
};

//--------------- Interface to be implemented by Event Source
struct OBSERVABLE {
   virtual bool Subscribe( OBSERVER * obs ) = 0;
    // did not implement UnSuscribe 
};

/////////////////////////////
// A toy implementation of Event Source
//
template<class T,class F,class M, class Marg, class Farg >
class EventSourceValueSubject : public OBSERVABLE {
   vector<OBSERVER> sinks; 
   T *State; 
   std::function<bool(Farg)> filter_func;
   std::function<Marg(Marg)> map_func;
  public:
   EventSourceValueSubject(Expr *n,F&& filter, M&& mapper) {
       State = n; map_func = mapper; filter_func = filter; NotifyAll(); 
   }
   ~EventSourceValueSubject() {  sinks.clear(); }
   virtual  bool Subscribe( OBSERVER  *sink ) { sinks.push_back(*sink); return true;}

   void NotifyAll() {
      double ret = Evaluate(State);
      list<double> ls; ls.push_back(ret);
      auto result = Map( ls, map_func);; // Apply Mapping Logic
      auto resulttr = Filter( result,filter_func); //Apply Filter
      if (resulttr.size() == 0 ) { return; }
      double dispatch_number = resulttr.front();
      for (auto sink : sinks) { 
           std::packaged_task<int()> task([&]() {
                  sink.ondata(dispatch_number); return 1;
           });
           std::future<int> result = task.get_future();task();
           double dresult = result.get();
         }
      }
      T* GetState() { return State; }
      void SetState(T *pstate) { State = pstate; NotifyAll(); }
};


Expr *getRandomExpr(int start, int end) {
    std::random_device rd;
    std::default_random_engine reng(rd());
    std::uniform_int_distribution<int> uniform_dist(start, end);
    double mean = uniform_dist(reng);
    return  new BinaryExpr( new Number(mean*1.0) , new Number(mean*2.0) , OPERATOR::PLUS);

}


int main( int argc, char **argv ){
   #if 0
     unique_ptr<Expr>  a(new BinaryExpr( new Number(10.0) , new Number(20.0) , OPERATOR::PLUS));
     unique_ptr<IExprVisitor> eval( new TreeEvaluatorVisitor());
     double result = a->accept(*eval);
     cout << "Output is => " << result << endl;
     unique_ptr<IExprVisitor>  exp(new ReversePolishEvaluator());
     a->accept(*exp);
     double result = Evaluate( &(*a));
     cout << result << endl;  
     //-------------- Extract all the Operands from the Expression Tree
     //-------------- ExprList(Expr *) will flatten the list and Filter will by applied
     auto cd = Filter( ExprList(&(*a)) , [](auto as) {  return as.knd != ExprKind::OPERATOR;} );
     //---------- Square the Value and Multiply by 3... used | as composition Operator
     //---------- See FuncCompose.h for details
     auto cdr = Map( cd, [] (auto s ) {  s.Value *=3; return s; } | [] (auto s ) { s.Value *= s.Value; return s; } );
     Iterate(cdr);
     cout << "Hello Evaluate " << Evaluate( &(*a)) << endl;

    
  #endif

     unique_ptr<Expr>  a(new BinaryExpr( new Number(10) , new Number(20) , OPERATOR::PLUS));
     EventSourceValueSubject<Expr,std::function<bool(double)>,
                    std::function<double(double)>,double,double> 
                    temp(&(*a),[] (auto s ) {   return s > 40.0;  },
                    []  (auto s ) { return s+ s ; }  |
                    []  (auto s ) { return s*2;} );
     
     OBSERVER obs_one ;
     obs_one.ondata = [](const double  r) {  cout << "*Final Value " <<  r << endl;};
     OBSERVER obs_two ;
     obs_two.ondata = [] ( const double r )  { cout << "**Final Value " << r << endl;};
     temp.Subscribe(&obs_one);
     temp.Subscribe(&obs_two);
     Expr *expr = 0;
     for( int i= 0; i < 10; ++i ) {
           cout << "--------------------------" <<  i << " "<< endl;
           expr = getRandomExpr(i*2, i*3 );
           temp.SetState(expr);
           std::this_thread::sleep_for(2s);
           delete expr;

     }
}


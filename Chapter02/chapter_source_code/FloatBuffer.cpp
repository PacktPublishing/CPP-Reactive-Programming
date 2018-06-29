//----- FloatBuffer.cpp
#include <iostream>
#include <vector>
using namespace std;
class FloatBuffer {
   double *bfr; int count;
 public:
   FloatBuffer():bfr(nullptr),count(0){}
   FloatBuffer(int pcount):bfr(new double[pcount]),count(pcount){}
    // Copy constructor.  
   FloatBuffer(const FloatBuffer& other)    : count(other.count)  
  , bfr(new double[other.count])  
   {     std::copy(other.bfr, other.bfr + count, bfr);   }  
  
   // Copy assignment operator – source code is obvious
   FloatBuffer& operator=(const FloatBuffer& other)  {  
      if (this != &other)   {  
         delete[] bfr;  // free memory of the current object
         count = other.count;  
         bfr   = new double[count]; //re-allocate 
         std::copy(other.bfr, other.bfr + count, bfr);  
      }  
      return *this;  
   }  
   // Move constructor to enable move semantics
   // The Modern STL containers supports move semetncis  
   FloatBuffer(FloatBuffer&& other)   : bfr(nullptr)   , count(0)  {  
   cout << "in move constructor" << endl;
   // since it is a move constructor, we are not copying elements from 
   // the source object. We just assign the pointers to steal memory
   bfr = other.bfr;  
   count = other.count;  
   // Now that we have grabbed our memory, we just assign null to 
   // source pointer 
   other.bfr = nullptr;  
   other.count = 0;  
}  
  
// Move assignment operator.  
FloatBuffer& operator=(FloatBuffer&& other)  {  
  
   if (this != &other)  
   {  
      // Free the existing resource.  
      delete[] bfr;  
   // Copy the data pointer and its length from the   
   // source object.  
   bfr = other.bfr;  
   count = other.count;  
  
   // We have stolen the memory, now set the pinter to null
   other.bfr = nullptr;  
   other.count = 0;  
   }  
   return *this;  
}  

};

int main() {
   // Create a vector object and add a few elements to it.  
   // Since STL supports move semantics move methods will be called.
   // in this particular case (Modern Compilers are smart)
   vector<FloatBuffer> v;  
   v.push_back(FloatBuffer(25));  
   v.push_back(FloatBuffer(75));  
  }

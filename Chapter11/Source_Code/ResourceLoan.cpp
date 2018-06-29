//----------- ResourceLoan.cpp
#include <rxcpp/rx.hpp>
using namespace std;
//////////////////////////
// implementation of Resource Loan
// Pattern. The Implementation opens a file
// and does not pass the file handle to user
// defined Lambda. The Ownership remains with
// the class 
//
class ResourceLoan {
   FILE *file;
   string filename;
  public:
     ResourceLoan(string pfile) {
        filename = pfile;
        file = fopen(filename.c_str(),"rb");
     }  
     ////////////////////////////
     // Read upto 1024 bytes to a buffer 
     // return the buffer contents and number of bytes
     int ReadBuffer( std::function<int(char pbuffer[],int val )> func )
     {
          if (file == nullptr ) { return -1; }
          char buffer[1024];
          int result = fread (buffer,1,1024,file);
          return func(buffer,result);
     } 
     //---------- close the resource
     ~ResourceLoan() { fclose(file);}
};
////////////////////////////////
// A Sample Program to invoke the above
// class
//
int main(int argc, char *argv[]) {
  ResourceLoan res("a.bin");
  int nread ;
  //------------- The conents of the buffer
  //------------- and size of buffer is stored in val
  auto rlambda =  [] (char buffer[] , int val ) {
       cout <<  "Size " << val << endl;
       return val;
  };
  //------- The File Handle is not available to the 
  //------- User defined Lambda
  while ((nread = res.ReadBuffer(rlambda)) > 0) {}
  //---- When the ResourceLoan object goes out of scope
  //---- File Handle is closed
  return 0;
}


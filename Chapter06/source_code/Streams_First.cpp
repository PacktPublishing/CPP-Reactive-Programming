#include "Stream.h"


using namespace std;
using namespace stream;
using namespace stream::op;

int main()
{


  int total = MakeStream::counter(1)
    | map_([] (int x) { return x * x; })
    | limit(10)
    | sum();

  cout << total << endl;


}
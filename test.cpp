#include <iostream>
#include <cassert>
#include "floatingClass.h"
using namespace std;

int main()
{
    FP x(31,25,"+1.01110101b+9");
    //cout << x.to_string() << endl;
   // assert(x.addOne());
    //cout << x.getSignificand() << endl;
    cout << x.to_string() << endl;
}



//|        ||
//|________|________|
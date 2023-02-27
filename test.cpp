#include <iostream>
#include <cassert>
#include "floatingClass.h"
using namespace std;

int main()
{
    FP nan(52,11);
    FP zero(52,11,0);
    FP one(52,11,1);
    FP two(52,11,2);
    FP inf(52,11,"+INF");

    FP x(two);
    x.add(one);         // x = 3
    cout << x.to_string();          // should print +1.1b+1

    FP y(two);
    y.negate();         // y = -2
    cout << y.to_string();          // should print -1.b+1

    FP z(x);
    z.add(y);           // z = 3-2 = -1
    z.negate();       // z = 1

    assert(zero.compare(zero) == FP::Order::EQUAL);
    //assert(x.compare(two) == FP::Order::AFTER);
    //assert(two.compare(x) == FP::Order::BEFORE);
    //assert(x.compare(nan) == FP::UNORDERED);
    //assert(x.compare(inf) == FP::BEFORE);
}



//|        ||
//|________|________|
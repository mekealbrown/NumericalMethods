#include <iostream>
#include <cassert>
#include "floatingClass.h"
using namespace std;

int main()
{
    FP nn(16,8,"+NAN");
    FP ni(16,8,"-INF");
    FP ny(16,8,-1032);
    FP nx(16,8,-363);
    FP nz(16,8,"-0");
    FP pz(16,8,"+0");
    FP px(16,8,363);
    FP py(16,8,1032);
    FP pi(16,8,"+INF");
        assert(ny.compare(nn) == FP::Order::UNORDERED);
        assert(ny.compare(ni) == FP::Order::AFTER);
        assert(ny.compare(ny) == FP::Order::EQUAL);
        assert(ny.compare(nx) == FP::Order::BEFORE);
        assert(ny.compare(nz) == FP::Order::BEFORE);
        assert(ny.compare(pz) == FP::Order::BEFORE);
        assert(ny.compare(px) == FP::Order::BEFORE);
        assert(ny.compare(py) == FP::Order::BEFORE);
        assert(ny.compare(pi) == FP::Order::BEFORE);
}



//|        ||
//|________|________|
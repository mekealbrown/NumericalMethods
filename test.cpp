#include <iostream>
#include <cassert>
#include "floatingClass.h"

int main()
{
    FP x(52,11, 129);
   // assert(x.getExponent() == 0x7ff); // depends on the size of the exponent
    //std::cout << "exponent " << x.getExponent() << "\n" << "sig " << x.getSignificand() << "\n";
    assert(!x.isNaN());
    assert(!x.isZero());
    assert(!x.isInfinity());
    assert(x.isNormal());
    assert(!x.isSubnormal());
    std::cout << x.to_string() << "\n";
}


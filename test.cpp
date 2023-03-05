#include <iostream>
#include <cassert>
#include "floatingClass.h"
using namespace std;

#define ASSERT_EQUAL(a,b) if ((a) != (b)) std::cout << "Assertion failed: " << (a) << " != " << (b) << " at:" << __LINE__ << "\n";
#define PRINT(a) cout << a << "\n";

int main()
{
    FP ny(16,8,-1516);
    FP nx(16,8,-892);
    FP nz(16,8,"-0");
    FP pz(16,8,"+0");
    FP px(16,8,892);
    FP py(16,8,1516);
    FP pd(16,8,624);
    FP t(px);           // == 892
    t.add(nz);          // == 892
   // ASSERT_EQUAL(t.compare(px), FP::Order::EQUAL);
    t.add(pz);          // == 892
   // ASSERT_EQUAL(t.compare(px), FP::Order::EQUAL);
    t.add(nx);          // 0
    //assert_var(t.isZero(),t.to_string());
    t.add(px);          // 892
   // ASSERT_EQUAL(t.compare(px), FP::Order::EQUAL);
    t.add(py);          // 2408
   // ASSERT_EQUAL(t.compare(py),FP::Order::AFTER);
   // ASSERT_EQUAL(t.compare(px),FP::Order::AFTER);
    t.add(nx);          // 1516 
   // ASSERT_EQUAL(t.compare(py),FP::Order::EQUAL); //this works
    t.add(nx);          // 624
   // PRINT(t.to_string());
   // PRINT(px.getSignificand());
   // ASSERT_EQUAL(t.compare(pd),FP::Order::EQUAL);


    t.add(ny);          // RZ(RZ(y-x)-y)
   // PRINT(t.to_string());
   // ASSERT_EQUAL(t.compare(nx),FP::Order::EQUAL);
   // t.add(ny);          // -y-x
   // t.add(px);          // -y
   // ASSERT_EQUAL(t.compare(ny), FP::Order::EQUAL);
   // ASSERT_EQUAL(t.compare(nz), FP::Order::BEFORE);
   // t.add(px);          // -y+x
   // t.add(py);          // +x
   // ASSERT_EQUAL(t.compare(px), FP::Order::EQUAL);
}



//|        ||
//|________|________|
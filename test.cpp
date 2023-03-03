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
    FP t(pz);           // +x
    //t.add(nz);          // +x
    //ASSERT_EQUAL(t.compare(px), FP::Order::EQUAL);
    //t.add(pz);          // +x
    //ASSERT_EQUAL(t.compare(px), FP::Order::EQUAL);
    //t.add(nx);          // 0
    //PRINT(px.to_string());
    //t.add(px);          // +x
    PRINT(t.to_string());
    PRINT(px.to_string());
   // ASSERT_EQUAL(t.compare(px), FP::Order::EQUAL);
    //t.add(py);          // x+y
    //ASSERT_EQUAL(t.compare(py),FP::Order::AFTER);
    //ASSERT_EQUAL(t.compare(px),FP::Order::AFTER);
    //t.add(nx);          // +y
    //ASSERT_EQUAL(t.compare(py),FP::Order::EQUAL);
    //t.add(nx);          // +y-x
    //ASSERT_EQUAL(t.compare(pd),FP::Order::EQUAL);
    //t.add(ny);          // RZ(RZ(y-x)-y)
    ////ASSERT_EQUAL(t.compare(nx),FP::Order::EQUAL);
    //t.add(ny);          // -y-x
    //t.add(px);          // -y
    //ASSERT_EQUAL(t.compare(ny), FP::Order::EQUAL);
    //ASSERT_EQUAL(t.compare(nz), FP::Order::BEFORE);
    //t.add(px);          // -y+x
    //t.add(py);          // +x
    //ASSERT_EQUAL(t.compare(px), FP::Order::EQUAL);
}



//|        ||
//|________|________|
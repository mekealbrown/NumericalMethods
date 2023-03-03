#include <iostream>
#include <cassert>
#include "floatingClass.h"
using namespace std;

int main()
{
    FP ny(16,8,-1516);
    FP nx(16,8,-892);
    FP nz(16,8,"-0");
    FP pz(16,8,"+0");
    FP px(16,8,892);
    FP py(16,8,1516);
    FP pd(16,8,624);
    FP t(px);           // +x
    t.add(nz);          // +x
    assert_equal(t.compare(px), FP::Order::EQUAL);
    t.add(pz);          // +x
    assert_equal(t.compare(px), FP::Order::EQUAL);
    t.add(nx);          // 0
    assert_var(t.isZero(),t.to_string());
    t.add(px);          // +x
    assert_equal(t.compare(px), FP::Order::EQUAL);
    t.add(py);          // x+y
    assert_equal(t.compare(py),FP::Order::AFTER);
    assert_equal(t.compare(px),FP::Order::AFTER);
    t.add(nx);          // +y
    assert_equal(t.compare(py),FP::Order::EQUAL);
    t.add(nx);          // +y-x
    assert_equal(t.compare(pd),FP::Order::EQUAL);
    t.add(ny);          // RZ(RZ(y-x)-y)
    //assert_equal(t.compare(nx),FP::Order::EQUAL);
    t.add(ny);          // -y-x
    t.add(px);          // -y
    assert_equal(t.compare(ny), FP::Order::EQUAL);
    assert_equal(t.compare(nz), FP::Order::BEFORE);
    t.add(px);          // -y+x
    t.add(py);          // +x
    assert_equal(t.compare(px), FP::Order::EQUAL);
}



//|        ||
//|________|________|
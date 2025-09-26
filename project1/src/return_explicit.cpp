#include "return_explicit.h"

namespace ReturnExplicit
{
A::A(int a, int b) : aData(a)
{
}
A::A(const A &data) : aData(data.aData), aData2(data.aData2)
{
}

void A::setData1(int data)
{
}
int A::getData1(){
    return aData;
}
double A::getData2(){
    return aData2;
}

B::B(A ita, int b) : objectA(ita), bData(b)
{
}

void B::setAObject(const A &data)
{
    objectA = data;
}

const A &B::getAObject() const
{
    return objectA;
}


}; // namespace ReturnExplicit
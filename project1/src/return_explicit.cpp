#include "return_explicit.h"

namespace ReturnExplicit
{

A::A(const A &data) : aData(data.aData), aData2(data.aData2)
{
}

A::A(int a = 0, int b = 0.0F, vector<uint16_t> vec = {0U})
    : aData(a), aData2(b), vec(vec)
{
}

void A::setData1(const int data)
{
    aData = data;
}
void A::setData2(double data)
{
    aData2 = data;
}
int A::getData1() const
{
    return aData;
}
double A::getData2() const
{
    return aData2;
}

void A::setVec(const vector<uint16_t> &data)
{
    vec = data;
}
vector<uint16_t> A::getVec() const
{
    return vec;
}

B::B(A ita, int b) : objectA(ita), bData(b)
{
}

// B &B::operator=(const B &data)
// {
//     objectA = data.objectA;
//     bData = data.bData;
//     return *this;
// }

void B::setbData(const double &data)
{
    bData = data;
}
double B::getbData() const
{
    return bData;
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
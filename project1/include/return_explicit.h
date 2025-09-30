#ifndef RETURN_EXPLICIT_H
#define RETURN_EXPLICIT_H

#include <vector>
#include <cstdint>
namespace ReturnExplicit
{
using std::uint16_t;
using std::vector;
class A    // 类A的定义
{
private:
    int aData;
    double aData2; 
    vector<uint16_t> vec;
public:
    A() = default;
    A(int a = 0, int b = 0.0F, vector<uint16_t> vec = {0U});
    explicit A(const A& data);
    ~A() = default;

    void setData1(const int data);
    void setData2(const double data);
    int getData1() const;
    double getData2() const;
    void setVec(const vector<uint16_t> &data);
    vector<uint16_t> getVec() const;
};

class B
{
public:
    int bData;

    B() = default;
    ~B() = default;
    B(A ita, int b = 0);
    B(const B &data);            // 构造函数声明
    B &operator=(const B &data) = default; // 赋值运算符声明

    void setbData(const double &data);
    double getbData() const;
    void setAObject(const A &data);
    const A &getAObject() const;

private:
    double bData2;
    A objectA;
};

}; // namespace ReturnExplicit
#endif /* RETURN_EXPLICIT_H */
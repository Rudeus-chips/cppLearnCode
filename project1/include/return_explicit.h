
#ifndef RETURN_EXPLICIT_H
#define RETURN_EXPLICIT_H

#include <vector>
namespace ReturnExplicit
{

class A    // 类A的定义
{
private:
    int aData;
    double aData2; 
public:
    A() = default;
    A(int a, int b = 0.0F);
    explicit A(const A& data);
    ~A() = default;

    void setData1(int data);
    void setData2(double data);
    int getData1();
    double getData2();

};

class B
{
public:
    int bData;

    A objectA;

    B() = default;
    B(A ita, int b = 0);
    explicit B(const B &); // 构造函数声明
    ~B() = default;

    void setAObject(const A &data);
    const A &getAObject() const;

private:
    double bData2;
};

}; // namespace ReturnExplicit
#endif /* RETURN_EXPLICIT_H */
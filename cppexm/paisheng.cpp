#include <iostream>
#include <cmath>

using namespace std;

class A
{
private:
    int a;

public:
    A() { cout << "A的构造函数" << endl; }
    ~A() { cout << "A的析构函数" << endl; }
};

class B : public A
{
private:
    int a;

public:
    B() { cout << "B的构造函数" << endl; }
    ~B() { cout << "B的析构函数" << endl; }

protected:
    void fun() { cout << "B的fun函数" << endl; }
};

int main()
{

    return 0;
}

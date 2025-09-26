#include <cstring>
#include <iostream>

class MyString {
private:
    char* data;

public:
    // 构造函数，将传入的字符串复制到data中
    MyString(const char* str) {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    // 析构函数，释放data指向的内存
    ~MyString() { delete[] data; }

    // 转换为 const char*（兼容 C 风格字符串）
    operator const char* const() {
        std::cout << "operator const char*() called, returning: " << data << std::endl;
        return data;
    }
    operator int() const {
        std::cout << "operator const int() called!" << std::endl;
        return 1;
    }
};

int main() {
    MyString str("Hello, C++!");
    printf("%d\n", str); // 隐式调用 operator const char*()
    std::cout << (const char*)str << std::endl;
    std::cout << (int)str << std::endl;
    std::cout << static_cast<const char*>(str) << std::endl;

    const int x = str;
    std::cout << x << std::endl;
    // printf("hello\n");
    // std::cout << str << std::endl;
    return 0;
}
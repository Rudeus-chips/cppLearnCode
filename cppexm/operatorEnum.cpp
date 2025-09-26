#include <iostream>

enum class Color { Red = 1, Green = 2, Blue = 3 };

// 包装类：封装 Color 枚举值
class ColorWrapper {
private:
    Color color;

public:
    explicit ColorWrapper(Color c) : color(c) {}

    // 类型转换运算符：成员函数形式（正确）
    operator int() const {
        return static_cast<int>(color);
    }
    // 如加上 explicit 关键字，则只能显式调用 value1无法编译通过
    // explicit operator int() const {
    //     return static_cast<int>(color);
    // }
};

int main() {

    // 后面有“::”的名称一定是类名或命名空间名
    Color c = Color::Green;
    ColorWrapper wrapper(c);  // 用包装类封装枚举值

    int value1 = wrapper;  // 隐式调用 operator int()
    int value2 = static_cast<int>(wrapper);  // 显式调用 operator int()
    
    std::cout << "Color value (wrapper implicit): " << value1 << std::endl;  // 输出：2
    std::cout << "Color value (wrapper explicit): " << value2 << std::endl;  // 输出：2
    std::cout << "__cplusplus = " << __cplusplus << std::endl;
    return 0;
}
#include <functional>
#include <iostream>

void increment(int& x) {
    x++;
}

int main() {
    int num = 0;
    // 绑定引用参数（使用 std::ref）
    auto bound_inc = std::bind(increment, std::ref(num));
    // 绑定常量引用参数（使用 std::cref）
    // auto bound_inc = std::bind(increment, std::cref(num));

    bound_inc();  // 调用后 num 变为 1
    std::cout << "num = " << num << std::endl;  // 输出 1

    return 0;
}
#include <iostream>
#include <vector>
#include <algorithm> // 用于 std::sort

int main()
{
    std::vector<int> nums = {3, 1, 4, 1, 5};
    int threshold = 3; // 外部作用域变量

    // Lambda 表达式：
    // - 捕获列表 [threshold]：值捕获外部变量 threshold
    // - 参数列表 (a, b)：两个 int 参数
    // - 返回类型：自动推导为 bool（可省略 -> bool）
    // - 函数体：判断 a 是否大于 threshold 且大于 b
    std::sort(nums.begin(), nums.end(), [threshold](int a, int b) { return (a > threshold) && (a < b); });

    for (int num : nums)
    {
        std::cout << num << " "; // 输出：5 4 3 1 1（排序逻辑：优先大于 threshold 的数降序，其余降序）
    }
    return 0;
}
#include<iostream>
#include<vector>

template <typename T>
class ArrayProxy {
private:
    T* array;
    size_t size;

public:
    ArrayProxy(T* arr, size_t sz) : array(arr), size(sz) {}

    // 重载下标运算符
    T& operator[](size_t index) {
        std::cout << "T& operator[] pass" << std::endl;
        return array[index];
    }

    // 转换为原始数组指针（透明访问）
    operator T*() {
        std::cout << "operator T*() pass" << std::endl;
        return array;
    }

    size_t get_size() const { return size; }
};

int main() {
    int raw_array[5] = {1, 2, 3, 4, 5};
    ArrayProxy<int> proxy(raw_array, 5);

    // 通过代理对象修改原始数组（透明访问）
    std::cout << "log1" << std::endl;
    proxy[2] = 100;  // 等价于 raw_array[2] = 100
    std::cout << "log2" << std::endl;
    // 直接传递代理对象给需要 int* 的函数
    void print_array(int* arr, size_t size);
    std::cout << "log3" << std::endl;
    print_array(proxy, proxy.get_size());  // 隐式转换为 int*
    std::cout << "log4" << std::endl;
    return 0;
}

void print_array(int* arr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    // 输出：1 2 100 4 5
}
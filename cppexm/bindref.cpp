#include <functional>
#include <iostream>

void increment(int& x) {
    x++;
}

int main() {
    int num = 0;
    // �����ò�����ʹ�� std::ref��
    auto bound_inc = std::bind(increment, std::ref(num));
    // �󶨳������ò�����ʹ�� std::cref��
    // auto bound_inc = std::bind(increment, std::cref(num));

    bound_inc();  // ���ú� num ��Ϊ 1
    std::cout << "num = " << num << std::endl;  // ��� 1

    return 0;
}
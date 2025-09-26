#include <functional>
#include <iostream>

class Button {
public:
    using ClickCallback = std::function<void()>;  // ����ص�����

    void set_click_callback(ClickCallback cb) {
        click_cb_ = std::move(cb);  // �洢�ص�
    }

    void click() {  // ģ�ⰴť����¼�
        if (click_cb_) {
            click_cb_();  // �����ص�
        }
    }

private:
    ClickCallback click_cb_;
};

int main() {
    Button btn;

    // ʹ�� Lambda ��Ϊ�ص�
    btn.set_click_callback([]() {
        std::cout << "Button clicked!" << std::endl;
    });

    btn.click();  // �����Button clicked!

    return 0;
}
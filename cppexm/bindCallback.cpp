#include <functional>
#include <iostream>

class Button {
public:
    using ClickCallback = std::function<void()>;  // 定义回调类型

    void set_click_callback(ClickCallback cb) {
        click_cb_ = std::move(cb);  // 存储回调
    }

    void click() {  // 模拟按钮点击事件
        if (click_cb_) {
            click_cb_();  // 触发回调
        }
    }

private:
    ClickCallback click_cb_;
};

int main() {
    Button btn;

    // 使用 Lambda 作为回调
    btn.set_click_callback([]() {
        std::cout << "Button clicked!" << std::endl;
    });

    btn.click();  // 输出：Button clicked!

    return 0;
}
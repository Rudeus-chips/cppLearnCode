#include <functional>
#include <iostream>

class Button {
public:
    using ClickCallback = std::function<void()>;  // ??????????

    void set_click_callback(ClickCallback cb) {
        click_cb_ = std::move(cb);  // ?›¥???
    }

    void click() {  // ??????????
        if (click_cb_) {
            click_cb_();  // ???????
        }
    }

private:
    ClickCallback click_cb_;
};

int main() {
    Button btn;

    // ??? Lambda ??????
    btn.set_click_callback([]() {
        std::cout << "Button clicked!" << std::endl;
    });

    btn.click();  // ?????Button clicked!

    return 0;
}
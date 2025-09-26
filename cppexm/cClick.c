#include <stdio.h>

// 定义按钮结构体（简化成员名）
typedef struct Button {
    void (*onClick)(void*);  // 点击回调（简化函数名）
    void* data;              // 用户数据（简化变量名）
} Button;

// 设置点击回调（简化函数名）
void set_click(Button* btn, void (*cb)(void*), void* data) {
    btn->onClick = cb;
    btn->data = data;
}

// 触发按钮点击（简化函数名）
void click_btn(Button* btn) {
    if (btn->onClick) btn->onClick(btn->data);  // 直接调用回调（简化条件判断）
}

// 点击回调逻辑（简化函数名和操作）
void on_click(void* data) {
    (*(int*)data)++;  // 直接转换并递增点击次数
    printf("点击次数：%d\n", *(int*)data);
}

int main() {

    int count = 0;  // 初始化点击次数
    // 初始化按钮（使用复合字面量直接绑定回调和数据）
    Button btn = {.onClick = on_click, .data = &count};
    
    click_btn(&btn);  // 第一次点击（输出：点击次数：1）
    click_btn(&btn);  // 第二次点击（输出：点击次数：2）
    
    return 0;
}
// g++ .\cClick.c -o cClick -std=c++23 -lws2_32  
// .\cClick.exe

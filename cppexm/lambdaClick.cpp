#include <iostream>
#include <functional>
#include <memory>  // 引入智能指针支持
#include <utility> // 引入std::move
#include <cassert>
#include <cstdlib> // 包含 exit()
#include <fstream>
#include <iomanip>

#define CHECK_NULL(val) \
    CheckNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// 辅助函数：读取源文件指定行的内容
std::string read_line_from_file(const std::string &file_path, int line_num)
{
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        return "无法打开文件";
    }
    std::string line;
    for (int i = 1; i <= line_num; ++i)
    {
        if (!std::getline(file, line))
        {
            return "行号超出文件范围";
        }
    }
    return line;
}

template <typename T>
T *CheckNull(const char *file, int line, const char *names, T *ptr)
{

    if (ptr == NULL)
    {
        std::string error_line = read_line_from_file(file, line);
        std::cerr << file << ":" << line << "\n\033[1;91m[Error]:\033[0m" << names << " Must be non NULL!\n"
                  << std::setw(6) << std::right << line << " |" << error_line << std::endl;
        std::exit(EXIT_FAILURE); // 运行时退出
    }

    return ptr;
}

class Button
{
public:
    template <typename Func>
    void setOnClick(Func &&callback)
    {
        onClick = std::forward<Func>(callback);
    }
    void click()
    {
        if (onClick)
            onClick();
    } // 模拟点击事件触发
private:
    std::function<void()> onClick = []() {};
};

int main()
{
    Button btn;
    int clickCount = 0;

    // 使用 Lambda 作为回调
    btn.setOnClick([&clickCount]()
                   { 
        clickCount++; 
        std::cout << "按钮被点击，次数：" << clickCount << std::endl; });

    std::unique_ptr<Button> p; // 初始化为不指向任何对象
    // p.reset(&btn);             // 将 p 指向 btn
    CHECK_NULL(p.get());       // 解锁此行会触发编译阶段的空指针检测
    p.get()->click();          // 输出：按钮被点击，次数：1
    btn.click();               // 输出：按钮被点击，次数：1
    // std::cout<< "btn.click() :1 done"<<std::endl;
    printf("\033[31m这是红色文本\033[0m\n");
    btn.click(); // 输出：按钮被点击，次数：2
    return 0;
}

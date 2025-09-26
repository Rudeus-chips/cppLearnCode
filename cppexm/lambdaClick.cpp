#include <iostream>
#include <functional>
#include <memory>  // ��������ָ��֧��
#include <utility> // ����std::move
#include <cassert>
#include <cstdlib> // ���� exit()
#include <fstream>
#include <iomanip>

#define CHECK_NULL(val) \
    CheckNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// ������������ȡԴ�ļ�ָ���е�����
std::string read_line_from_file(const std::string &file_path, int line_num)
{
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        return "�޷����ļ�";
    }
    std::string line;
    for (int i = 1; i <= line_num; ++i)
    {
        if (!std::getline(file, line))
        {
            return "�кų����ļ���Χ";
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
        std::exit(EXIT_FAILURE); // ����ʱ�˳�
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
    } // ģ�����¼�����
private:
    std::function<void()> onClick = []() {};
};

int main()
{
    Button btn;
    int clickCount = 0;

    // ʹ�� Lambda ��Ϊ�ص�
    btn.setOnClick([&clickCount]()
                   { 
        clickCount++; 
        std::cout << "��ť�������������" << clickCount << std::endl; });

    std::unique_ptr<Button> p; // ��ʼ��Ϊ��ָ���κζ���
    // p.reset(&btn);             // �� p ָ�� btn
    CHECK_NULL(p.get());       // �������лᴥ������׶εĿ�ָ����
    p.get()->click();          // �������ť�������������1
    btn.click();               // �������ť�������������1
    // std::cout<< "btn.click() :1 done"<<std::endl;
    printf("\033[31m���Ǻ�ɫ�ı�\033[0m\n");
    btn.click(); // �������ť�������������2
    return 0;
}

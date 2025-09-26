#include <iostream>
#include <fstream>
#include <thread>
#include <functional>
#include <stdexcept>

// 定义回调函数类型：参数为读取的字节数和数据缓冲区（nullptr表示失败）
using ReadCallback = std::function<void(size_t bytes_read, const char* buffer)>;

/**
 * @brief 异步读取文件的函数（传统回调方式）
 * @param filename 文件名
 * @param callback 读取完成后调用的回调函数
 */
void async_read_file_traditional(const std::string& filename, ReadCallback callback) {
    // 启动一个新线程执行实际的文件读取操作（模拟异步）
    std::thread([filename, callback]() {
        try {
            std::ifstream file(filename, std::ios::binary);
            if (!file.is_open()) {
                // 读取失败时，通过回调传递0字节和nullptr
                callback(0, nullptr);
                return;
            }

            char buffer[1024]; // 读取缓冲区（固定大小）
            file.read(buffer, sizeof(buffer)); // 同步读取（实际异步应使用非阻塞API）
            size_t bytes_read = file.gcount(); // 获取实际读取的字节数

            // 读取完成后，调用回调函数传递结果
            callback(bytes_read, buffer);
        } catch (const std::exception& e) {
            // 异常时通过回调传递错误信息（此处简化为0字节）
            callback(0, nullptr);
        }
    }).detach(); // 分离线程，不阻塞当前线程
}

int main() {
    // 调用传统异步读取函数，传入回调处理结果
    async_read_file_traditional("example.txt", 
        [](size_t bytes_read, const char* buffer) {
            if (bytes_read == 0) {
                std::cerr << "Error: Failed to read file or empty file." << std::endl;
                return;
            }
            std::cout << "传统异步读取结果（" << bytes_read << "字节）: " << std::endl;
            std::cout.write(buffer, bytes_read); // 输出读取的数据
            std::cout << std::endl;
        });

    // 主线程继续执行其他操作（不阻塞）
    std::cout << "主线程继续运行..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // 模拟主线程继续执行
    std::cout << "主线程继续运行2" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 等待异步操作完成

    return 0;
}
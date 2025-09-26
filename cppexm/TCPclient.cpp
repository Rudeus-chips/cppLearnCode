#include <iostream>       // 输入输出流，用于控制台交互
#include <string>          // 字符串处理库
#include <cstring>         // C风格字符串操作函数
#include <winsock2.h>     // Windows套接字API核心头文件
#include <ws2tcpip.h>     // 提供IP地址转换等扩展功能
#include <limits>      
#pragma comment(lib, "ws2_32.lib")  // 链接Winsock库，用于网络通信

int main() {
    // ============================ Windows网络初始化 ============================
    WSADATA wsaData;  // 存储Winsock库初始化信息的数据结构
    
    // 初始化Winsock库，指定使用2.2版本
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        // 初始化失败处理
        std::cerr << "WSAStartup 失败，错误码: " << WSAGetLastError() << std::endl;
        return 1;  // 返回错误代码终止程序
    }

    // ============================ 创建TCP套接字 ============================
    // 创建IPv4协议的TCP套接字
    SOCKET client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // 检查套接字是否创建成功
    if (client_fd == INVALID_SOCKET) {
        std::cerr << "socket 创建失败，错误码: " << WSAGetLastError() << std::endl;
        WSACleanup();  // 清理Winsock资源
        return 1;      // 返回错误代码
    }

    // 设置套接字超时选项（防止操作阻塞无响应）
    DWORD timeout = 3000; // 3秒超时（单位：毫秒）
    // 设置接收超时
    setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    // 设置发送超时
    setsockopt(client_fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

    // ============================ 连接服务器 ============================
    sockaddr_in server_addr;  // 定义服务器地址结构
    memset(&server_addr, 0, sizeof(server_addr));  // 清空内存
    server_addr.sin_family = AF_INET;       // 使用IPv4协议
    server_addr.sin_port = htons(8080);     // 设置目标端口（主机字节序转网络字节序）

    // 将点分十进制IP(127.0.0.1)转换为网络格式的二进制地址
    if (inet_pton(AF_INET, "127.0.0.1", &(server_addr.sin_addr)) <= 0) {
        std::cerr << "inet_pton 失败，错误码: " << WSAGetLastError() << std::endl;
        closesocket(client_fd);  // 关闭套接字
        WSACleanup();           // 清理Winsock资源
        return 1;              // 返回错误代码
    }

    // 尝试连接到服务器
    if (connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "connect 失败，错误码: " << WSAGetLastError() << std::endl;
        closesocket(client_fd);
        WSACleanup();
        return 1;
    }

    // 连接成功后显示提示信息
    std::cout << "成功连接到服务器。输入消息发送给服务器(输入'exit'退出)：" << std::endl;

    // ============================ 主通信循环 ============================
    char buffer[1024] = {0};  // 接收缓冲区
    
    while (true) {
        // 获取用户输入
        std::cout << "客户端消息: ";
        std::string userInput;
        std::getline(std::cin, userInput);  // 读取整行输入
        
        // 退出条件检查：用户输入"exit"时退出循环
        if (userInput == "exit") {
            break;
        }

        // ============================ 发送数据 ============================
        // 将用户输入发送到服务器
        int send_len = send(client_fd, userInput.c_str(), userInput.length(), 0);
        
        if (send_len == SOCKET_ERROR) {  // 发送失败处理
            int error = WSAGetLastError();  // 获取具体错误代码
            
            if (error == WSAETIMEDOUT) {  // 超时错误处理
                std::cerr << "发送超时，是否继续尝试? (y/n): ";
                std::string choice;
                std::getline(std::cin, choice);  // 获取用户选择
                if (choice != "y" && choice != "Y") break;  // 用户选择退出
            } else {  // 其他错误处理
                std::cerr << "send 失败，错误码: " << error << std::endl;
                break;  // 退出循环
            }
        } else {  // 发送成功反馈
            std::cout << "已发送数据: " << userInput << " (" << send_len << "字节)" << std::endl;
        }

        // ============================ 接收响应 ============================
        memset(buffer, 0, sizeof(buffer));  // 清空接收缓冲区
        // 接收服务器响应（保留1字节位置用于字符串终止符）
        int recv_len = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        
        if (recv_len > 0) {  // 接收到数据
            buffer[recv_len] = '\0';  // 添加字符串结束符
            std::cout << "服务器响应: " << buffer << std::endl;
        } else if (recv_len == 0) {  // 连接已关闭
            std::cout << "服务器已关闭连接" << std::endl;
            break;  // 退出循环
        } else {  // 接收错误处理
            int error = WSAGetLastError();
            if (error == WSAETIMEDOUT) {  // 接收超时
                std::cerr << "接收超时，服务器未响应" << std::endl;
            } else {  // 其他错误
                std::cerr << "recv 失败，错误码: " << error << std::endl;
                break;  // 退出循环
            }
        }
    }

    // ============================ 清理资源 ============================
    closesocket(client_fd);  // 关闭套接字
    WSACleanup();            // 清理Winsock资源
    std::cout << "按回车键退出...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 等待用户回车
    return 0;  // 程序正常退出
}
                                                              
// g++ .\TCPclient.cpp -o client -std=c++23 -lws2_32  
// .\client.exe
// chcp 65001
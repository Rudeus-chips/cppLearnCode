#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
    // 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup 失败，错误码: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // 1. 创建监听套接字
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket 创建失败，错误码: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 2. 配置地址和端口
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // 接受任意IP的连接
    serverAddr.sin_port = htons(8080);       // 监听8080端口

    // 3. 绑定套接字到地址
    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind 失败，错误码: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // 4. 开始监听
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen 失败，错误码: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "服务器已启动，等待客户端连接... (端口: 8080)" << std::endl;

    // 5. 接受客户端连接
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    
    SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "accept 失败，错误码: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // 获取客户端IP
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
    std::cout << "客户端已连接，IP地址: " << clientIP << std::endl;

    // 6. 接收客户端数据
    char recvBuffer[1024];
    int recvBytes;
    
    while ((recvBytes = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0)) > 0) {
        recvBuffer[recvBytes] = '\0';  // 添加字符串终止符
        
        std::cout << "接收到 " << recvBytes << " 字节数据: " << recvBuffer << std::endl;
        
        // 7. 可选：发送响应
        const char* response = "服务器已收到你的消息!";
        send(clientSocket, response, strlen(response), 0);
        
        // 如果只想接收一次消息，可以在此处break
        // break;
    }

    if (recvBytes == 0) {
        std::cout << "客户端关闭了连接" << std::endl;
    } else {
        std::cerr << "recv 失败，错误码: " << WSAGetLastError() << std::endl;
    }

    // 8. 关闭套接字和清理
    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();
    
    std::cout << "按 Enter 退出...";
    std::cin.get();
    return 0;
}

// g++ .\TCPserver.cpp -o server -std=c++23 -lws2_32  
// .\server.exe

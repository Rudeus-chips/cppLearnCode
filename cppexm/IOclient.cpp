#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1";
const int BUFFER_SIZE = 1024;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "connect failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server. Type messages (type 'exit' to quit):" << std::endl;

    while (true) {
        std::string message;
        std::getline(std::cin, message);
        if (message == "exit") break;

        send(sock, message.c_str(), message.size(), 0);
        
        char buffer[BUFFER_SIZE];
        int bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                std::cout << "Server disconnected." << std::endl;
            } else {
                std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
            }
            break;
        }
        buffer[bytes_received] = '\0';
        std::cout << "Echo: " << buffer << std::endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
// g++ .\IOclient.cpp -o IOclient -std=c++23 -lws2_32  
// .\IOclient.exe
// chcp 65001
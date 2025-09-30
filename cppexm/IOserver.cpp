#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>

#pragma comment(lib, "ws2_32.lib")

const int PORT = 8080;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 64;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::vector<SOCKET> client_sockets;
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(server_fd, &read_fds);

    std::cout << "Server started on port " << PORT << std::endl;

    while (true) {
        fd_set temp_fds = read_fds;
        int socket_count = select(0, &temp_fds, nullptr, nullptr, nullptr);
        if (socket_count == SOCKET_ERROR) {
            std::cerr << "select failed: " << WSAGetLastError() << std::endl;
            break;
        }

        for (int i = 0; i < socket_count; i++) {
            SOCKET sock = temp_fds.fd_array[i];
            if (sock == server_fd) {
                // 有新连接
                sockaddr_in client_addr{};
                int addr_len = sizeof(client_addr);
                SOCKET client_fd = accept(server_fd, (sockaddr*)&client_addr, &addr_len);
                if (client_fd == INVALID_SOCKET) {
                    std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
                    continue;
                }

                if (client_sockets.size() < MAX_CLIENTS) {
                    client_sockets.push_back(client_fd);
                    FD_SET(client_fd, &read_fds);
                    char ip[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &client_addr.sin_addr, ip, INET_ADDRSTRLEN);
                    std::cout << "New client connected: " << ip << std::endl;
                } else {
                    std::cout << "Too many clients. Connection refused." << std::endl;
                    closesocket(client_fd);
                }
            } else {
                // 客户端数据
                char buffer[BUFFER_SIZE];
                int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
                if (bytes_received <= 0) {
                    if (bytes_received == 0) {
                        std::cout << "Client disconnected." << std::endl;
                    } else {
                        std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
                    }
                    closesocket(sock);
                    FD_CLR(sock, &read_fds);
                    // 从客户端列表中移除
                    for (auto it = client_sockets.begin(); it != client_sockets.end(); ++it) {
                        if (*it == sock) {
                            client_sockets.erase(it);
                            break;
                        }
                    }
                } else {
                    buffer[bytes_received] = '\0';
                    std::cout << "Received: " << buffer;
                    
                    // 回显数据
                    send(sock, buffer, bytes_received, 0);
                }
            }
        }
    }

    // 清理
    for (SOCKET sock : client_sockets) {
        closesocket(sock);
    }
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
// g++ .\IOserver.cpp -o IOserver -std=c++23 -lws2_32  
// .\IOserver.exe

#include "server.h"
#include "util.h"
#include <cstring>
#include <ctime>
#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

std::wstring correct_id = L"202226910925"; // 替换为你自己的学号
std::wstring correct_name = L"杨俊科";   // 替换为你的姓名

// Server 构造函数，初始化端口和线程池
Server::Server(int port)
    : port(port), thread_pool(4) {} // 使用线程池，最多4个线程

void Server::start() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::wcerr << L"WSAStartup failed!" << std::endl;
        return;
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        std::wcerr << L"Socket creation failed!" << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
      std::cerr << "Failed to set SO_REUSEADDR with error code: " << WSAGetLastError() << std::endl;
    }
 


    if (bind(server_socket, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == SOCKET_ERROR) {
        int error_code = WSAGetLastError();
        std::wcerr << L"Bind failed with error code: " << error_code
                   << std::endl;

        closesocket(server_socket);
        WSACleanup();
        return;
    }

    if (listen(server_socket, 5) == SOCKET_ERROR) {
        std::wcerr << L"Listen failed!" << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    std::wcout << L"Server listening on port " << port << std::endl;

    while (true) {
        SOCKET client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket == INVALID_SOCKET) {
            std::wcerr << L"Accept failed!" << std::endl;
            continue;
        }

        // 使用线程池提交处理客户端请求的任务
        thread_pool.submit(
            [this, client_socket] { handle_client(client_socket); });
    }

    closesocket(server_socket);
    WSACleanup();
}

void Server::handle_client(SOCKET client_socket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        closesocket(client_socket);
        return;
    }

    // 接收到的字节流直接转换为 std::wstring
    std::wstring wrequest(reinterpret_cast<wchar_t *>(buffer),
                          bytes_received / 2); // 每两个字节为一个 wchar_t

    // 处理请求
    std::wstring wresponse = process_request(wrequest);

    // 将 wstring 转回 UTF-16LE 字节流
    int response_size = wresponse.size() * sizeof(wchar_t);
    char response[1024];
    memcpy(response, wresponse.c_str(), response_size);

    send(client_socket, response, response_size, 0);
    closesocket(client_socket);
}

std::wstring Server::process_request(const std::wstring &request) {
    if (request == L"gettime") {
        return get_current_time();

    } else if (request.rfind(L"test ", 0) == 0) {
        return request.substr(5);

    } else if (request.rfind(L"auth ", 0) == 0) {
        std::wstring student_id = request.substr(5);
        return (student_id == correct_id) ? L"ok" : L"error";

    } else if (request.rfind(L"auth2 ", 0) == 0) {
        std::wstring credentials = request.substr(6);
        size_t comma_pos = credentials.find(L',');
        if (comma_pos != std::wstring::npos) {
            std::wstring student_id = credentials.substr(0, comma_pos);
            std::wstring name = credentials.substr(comma_pos + 1);
            return (student_id == correct_id && name == correct_name)
                       ? L"ok"
                       : L"error";
        } else {
            return L"error";
        }
    }
    return L"unknown command";
}

// Server 析构函数，关闭线程池
Server::~Server() {
    thread_pool.shutdown(); // 等待所有任务完成后关闭线程池
}

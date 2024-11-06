#include "server.h"
#include "util.h"
#include <cstring>
#include <iostream>
const std::string correct_id = "202226910925";
const std::string correct_name = "杨俊科";

// 构造函数，初始化端口和线程池
Server::Server(int port) : port(port), thread_pool(4) {}

void Server::start() {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    std::cerr << "Socket creation failed!" << std::endl;
    return;
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    std::cerr << "Bind failed!" << std::endl;
    close(server_socket);
    return;
  }

  listen(server_socket, 5);
  std::cout << "Server listening on port " << port << std::endl;

  while (true) {
    socket_t client_socket = accept(server_socket, nullptr, nullptr);
    if (client_socket < 0) {
      std::cerr << "Accept failed!" << std::endl;
      continue;
    }

    // 使用线程池提交任务
    thread_pool.submit([this, client_socket] { handle_client(client_socket); });
  }

  close(server_socket);
}

void Server::handle_client(socket_t client_socket) {
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));

  ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
  if (bytes_received <= 0) {
    close(client_socket);
    return;
  }

  buffer[bytes_received] = '\0';
  std::string request(buffer);
  std::string response = process_request(request);

  send(client_socket, response.c_str(), response.length(), 0);
  close(client_socket);
}

std::string Server::process_request(const std::string &request) {
  if (request == "gettime") {
    return get_current_time();
  } else if (request.rfind("test ", 0) == 0) {
    return request.substr(5);
  } else if (request.rfind("auth ", 0) == 0) {
    std::string student_id = request.substr(5);
    return (student_id == correct_id) ? "ok" : "error";
  } else if (request.rfind("auth2 ", 0) == 0) {
    std::string credentials = request.substr(6);
    size_t comma_pos = credentials.find(',');
    if (comma_pos != std::string::npos) {
      std::string student_id = credentials.substr(0, comma_pos);
      std::string name = credentials.substr(comma_pos + 1);
      return (student_id == correct_id && name == correct_name) ? "ok"
                                                                : "error";
    } else {
      return "error";
    }
  }
  return "unknown command";
}

Server::~Server() {
  thread_pool.shutdown(); // 等待所有任务完成后关闭线程池
}

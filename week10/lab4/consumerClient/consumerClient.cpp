#include "consumerClient.h"
#include <arpa/inet.h> // 处理 IP 地址的转换
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

ConsumerClient::ConsumerClient(const std::string &server_ip, int port)
    : server_ip_(server_ip), port_(port), sock_(-1) {}

void ConsumerClient::connect_to_server() {
  // 创建Socket
  sock_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_ < 0) {
    perror("Socket creation failed");
    throw std::runtime_error("Socket creation failed");
  }

  // 设置服务器地址
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = port_; // htons(port_);

  // 将服务器IP地址从文本格式转换为二进制格式
  if (inet_pton(AF_INET, server_ip_.c_str(), &serv_addr.sin_addr) <= 0) {
    perror("Invalid address");
    close(sock_);
    throw std::runtime_error("Invalid server address");
  }

  // 尝试连接到服务器
  if (connect(sock_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Connection to server failed");
    close(sock_);
    throw std::runtime_error("Connection to server failed");
  }

  std::cout << "Connected to server at " << server_ip_ << ":" << port_
            << std::endl;

  // 发送消费者标识符到服务器
  std::string role = "consumer";
  ssize_t bytes_sent = send(sock_, role.c_str(), role.size(), 0);
  if (bytes_sent == -1) {
    perror("Failed to send role to server");
    close(sock_);
    throw std::runtime_error("Failed to send role to server");
  }

  std::cout << "Sent role 'consumer' to server." << std::endl;
}

void ConsumerClient::consume_data() {
  char buffer[1024] = {0}; // 用于接收数据的缓冲区

  while (true) {
    ssize_t bytes_received = recv(sock_, buffer, sizeof(buffer), 0);

    if (bytes_received < 0) {
      // 接收错误处理
      std::cerr << "Error receiving data from server" << std::endl;
      break; // 停止接收数据
    }

    if (bytes_received == 0) {
      // 服务器关闭连接
      std::cout << "Server disconnected or no data available." << std::endl;
      break; // 停止接收数据
    }

    // 输出接收到的数据
    std::cout << "Received data: " << std::string(buffer, bytes_received)
              << std::endl;
  }
}

void ConsumerClient::start() {
  try {
    connect_to_server(); // 连接到服务器
    consume_data();      // 开始接收数据
  } catch (const std::exception &e) {
    std::cerr << "Error in ConsumerClient: " << e.what() << std::endl;
  }
}

int main() {
  std::string server_ip = "127.0.0.1";
  int port = 6666;

  try {
    ConsumerClient consumer(server_ip, port);
    consumer.start(); // 启动消费者客户端，连接到服务器并消费数据
  } catch (const std::exception &e) {
    std::cerr << "Consumer client failed: " << e.what() << std::endl;
  }

  return 0;
}

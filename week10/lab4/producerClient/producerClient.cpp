#include "producerClient.h"
#include <arpa/inet.h> //处理ip地址的转换
#include <iostream>
#include <netinet/in.h>
#include <random>
#include <sys/socket.h>
#include <unistd.h>

void ProducerClient::connect_to_server() {
  // 创建Socket
  sock_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_ < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // 设置服务器地址
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port_);

  // 将服务器IP地址从文本格式转换为二进制格式
  if (inet_pton(AF_INET, server_ip_.c_str(), &serv_addr.sin_addr) <= 0) {
    perror("Invalid address");
    close(sock_);
    exit(EXIT_FAILURE);
  }

  // 尝试连接到服务器
  if (connect(sock_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Connection to server failed");
    close(sock_);
    exit(EXIT_FAILURE);
  }

  std::cout << "Connected to server at " << server_ip_ << ":" << port_
            << std::endl;
}

void ProducerClient::produce_data() {
    if (sock_ == -1) {
        std::cerr << "Not connected to server!" << std::endl;
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(2000000001, std::numeric_limits<int>::max());
    
    int num = 1000000; //发送100万个数字
    for (int i = 0; i < num; ++i) {
        int random_number = dist(gen);
        ssize_t bytes_sent = send(sock_, &random_number, sizeof(random_number), 0);
        if (bytes_sent == -1) {
            std::cerr << "Error sending data to server" << std::endl;
            break;
        }
    }
}
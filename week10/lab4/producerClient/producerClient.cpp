#include "producerClient.h"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <random>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

ProducerClient::ProducerClient(const std::string &server_ip, int port)
    : server_ip_(server_ip), port_(port), sock_(-1) {}

void ProducerClient::connect_to_server() {
  sock_ = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_ < 0) {
    perror("Socket creation failed");
    throw std::runtime_error("Socket creation failed");
  }

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port_);

  if (inet_pton(AF_INET, server_ip_.c_str(), &serv_addr.sin_addr) <= 0) {
    perror("Invalid address");
    close(sock_);
    throw std::runtime_error("Invalid server address");
  }

  if (connect(sock_, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Connection to server failed");
    close(sock_);
    throw std::runtime_error("Connection to server failed");
  }

  std::cout << "Connected to server at " << server_ip_ << ":" << port_
            << std::endl;

  std::string role = "producer\n"; // 添加换行符作为结束标志
  ssize_t bytes_sent = send(sock_, role.c_str(), role.size(), 0);
  if (bytes_sent == -1) {
    perror("Failed to send role to server");
    close(sock_);
    throw std::runtime_error("Failed to send role to server");
  }
  std::cout << "Sent role 'producer' to server." << std::endl;
}

void ProducerClient::produce_data() {
  if (sock_ == -1) {
    std::cerr << "Not connected to server!" << std::endl;
    return;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(2000000001,
                                       std::numeric_limits<int>::max());

  int num = 50;
  std::vector<int> buffer;
  buffer.reserve(100);

  for (int i = 0; i < num; ++i) {
    int random_number = dist(gen);
    buffer.push_back(random_number);

    if (buffer.size() == 10) {
      ssize_t bytes_sent =
          send(sock_, buffer.data(), buffer.size() * sizeof(int), 0);
      if (bytes_sent == -1) {
        std::cerr << "Error sending data to server" << std::endl;
        break;
      }
      buffer.clear();
    }
  }

  if (!buffer.empty()) {
    send(sock_, buffer.data(), buffer.size() * sizeof(int), 0);
  }
}

void ProducerClient::start() {
  connect_to_server();
  produce_data();
}

int main() {
  std::string server_ip = "127.0.0.1";
  int port = 6666;

  try {
    ProducerClient producer(server_ip, port);
    producer.start();
  } catch (const std::exception &e) {
    std::cerr << "Producer client failed: " << e.what() << std::endl;
  }

  return 0;
}

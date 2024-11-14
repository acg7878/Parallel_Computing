#include "queueServer.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

void queueServer::start_server() {
  int server_fd;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("Listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  std::cout << "Server listening on port " << port << std::endl;

  std::vector<std::thread> threads;

  while (true) {
    int new_socket =
        accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
      perror("Accept failed");
      continue;
    }

    threads.emplace_back([this, new_socket]() {
      char buffer[1024] = {0};
      int valread = read(new_socket, buffer, 1024);
      std::string role(buffer, buffer + valread);
      role = role.substr(0, role.find('\n')); // 提取角色，直到换行符

      std::cout << "Received role: " << role << std::endl;

      if (role == "producer") {
        handle_producer(new_socket);
      } else if (role == "consumer") {
        handle_consumer(new_socket);
      } else {
        std::cerr << "Unknown role received: " << role << std::endl;
      }

      close(new_socket);
    });
  }

  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  close(server_fd);
}

void queueServer::handle_producer(int client_socket) {
  char buffer[1024] = {0};
  std::vector<int> received_data;

  while (true) {
    int valread = read(client_socket, buffer, sizeof(buffer));
    if (valread <= 0) {
      std::cerr << "Producer disconnected or error occurred." << std::endl;
      break;
    }

    for (int i = 0; i < valread / sizeof(int); ++i) {
      int msg;
      std::memcpy(&msg, buffer + i * sizeof(int), sizeof(int));
      // msg = ntohl(msg);
      received_data.push_back(msg);
    }

    if (received_data.size() >= 10) {
      for (int i = 0; i < 100; ++i) {
        push(received_data[i]);
        std::cout << "Received from producer: " << received_data[i]
                  << std::endl;
      }
      received_data.erase(received_data.begin(), received_data.begin() + 100);
    }
  }

  for (int i = 0; i < received_data.size(); ++i) {
    push(received_data[i]);
  }
}

void queueServer::handle_consumer(int client_socket) {
  while (true) {
    int msg = pop();
    msg = htonl(msg);
    ssize_t bytes_sent = send(client_socket, &msg, sizeof(msg), 0);
    if (bytes_sent == -1) {
      std::cerr << "Error sending data to consumer" << std::endl;
      break;
    }
    std::cout << "Sent to consumer: " << msg << std::endl;
  }
}

int queueServer::pop() {
  std::unique_lock<std::mutex> lock(mtx);
  cv.wait(lock, [this] { return !queue.empty(); });
  int item = queue.front();
  queue.pop();
  return item;
}

void queueServer::push(int item) {
  std::lock_guard<std::mutex> lock(mtx);
  queue.push(item);
  cv.notify_one();
}

void queueServer::start() { start_server(); }

int main() {
  int port = 6666;
  try {
    queueServer server(port);
    server.start();
  } catch (const std::exception &e) {
    std::cerr << "Queue server failed: " << e.what() << std::endl;
  }

  return 0;
}

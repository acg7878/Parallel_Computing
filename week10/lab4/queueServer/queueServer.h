#ifndef QUEUESERVER_H
#define QUEUESERVER_H

#include <arpa/inet.h>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sys/socket.h>
#include <unistd.h>

template <typename T> class queueServer {
public:
  queueServer(int port) : port(port) {}

  // 禁止拷贝构造和赋值操作
  queueServer(const queueServer &) = delete;
  queueServer &operator=(const queueServer &) = delete;

  // 启动服务端并处理客户端连接
  void start_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 创建服务器端Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
      perror("Socket failed");
      exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 绑定Socket到指定端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
      perror("Bind failed");
      exit(EXIT_FAILURE);
    }

    // 开始监听
    if (listen(server_fd, 3) < 0) {
      perror("Listen failed");
      exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
      // 接受客户端连接
      if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                               (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
      }

      char buffer[1024] = {0};
      int valread = read(new_socket, buffer, 1024);
      if (valread > 0) {
        std::string msg(buffer, valread);
        push(msg); // 将接收到的消息放入队列
        std::cout << "Received: " << msg << std::endl;
      }

      close(new_socket); // 关闭连接
    }

    close(server_fd); // 关闭服务器端套接字
  }

  // 从队列中取出元素
  T pop() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return !queue.empty(); }); // 队列为空时等待
    T item = queue.front();
    queue.pop();
    return item;
  }

  // 向队列添加元素
  void push(const T &item) {
    std::lock_guard<std::mutex> lock(mtx);
    queue.push(item);
    cv.notify_one(); // 通知消费者线程
  }

private:
  int port;
  std::queue<T> queue;        // 存储队列数据
  mutable std::mutex mtx;     // 用于同步访问队列
  std::condition_variable cv; // 用于队列为空时的等待机制
};

#endif // QUEUESERVER_H

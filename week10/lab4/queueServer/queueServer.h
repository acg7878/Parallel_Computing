#ifndef QUEUESERVER_H
#define QUEUESERVER_H

#include <arpa/inet.h>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <sys/socket.h>
#include <unistd.h>

class queueServer {
public:
  queueServer(int port) : port(port) {}

  // 禁止拷贝构造和赋值操作
  queueServer(const queueServer &) = delete;
  queueServer &operator=(const queueServer &) = delete;

  void start();

private:
  int port;
  std::queue<int> queue;      // 存储队列数据
  mutable std::mutex mtx;     // 用于同步访问队列
  std::condition_variable cv; // 用于队列为空时的等待机制
  void start_server();
  int pop();
  void push(int item);
  void handle_consumer(int client_socket);
  void handle_producer(int client_socket);
};

#endif // QUEUESERVER_H

#ifndef SERVER_H 
#define SERVER_H

#include <string>
#include "thread_pool.h" // 确保这里包含的是正确的线程池头文件

#ifdef _WIN32  // Windows
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET socket_t;
#else  // Linux
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int socket_t;
#endif

class Server {
public:
  Server(int port);  // 构造函数
  void start();      // 启动服务器
  ~Server();         // 析构函数

private:
  int port;  // 服务器端口
  void handle_client(socket_t client_socket);  // 处理客户端请求
  std::wstring process_request(const std::wstring& request);  // 请求处理函数，使用 std::wstring 类型
  thread_utils::thread_pool thread_pool;  // 线程池成员变量，使用默认构造函数
};

#endif // SERVER_H

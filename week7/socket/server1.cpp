#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

std::string getCurrentDateTime() {
  auto now = std::time(nullptr);
  std::tm *localTime = std::localtime(&now);
  std::ostringstream oss;
  oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}

int main() {
  // 1. 创建 Socket
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    std::cerr << "Socket creation failed!" << std::endl;
    return -1;
  }

  // 2. 服务器地址配置
  struct sockaddr_in address;
  address.sin_family = AF_INET;         // IPv4
  address.sin_addr.s_addr = INADDR_ANY; // 本地地址
  address.sin_port = htons(8888);       // 端口号

  // 3. 允许地址重用
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  // 4. 绑定 Socket 到地址
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    std::cerr << "Bind failed! Error: " << strerror(errno) << std::endl;
    close(server_fd);
    return -1;
  }

  // 5. 开始监听
  if (listen(server_fd, 3) < 0) {
    std::cerr << "Listen failed! Error: " << strerror(errno) << std::endl;
    close(server_fd);
    return -1;
  }
  std::cout << "Server listening on port 8888..." << std::endl;

  // 6. 接受客户端连接
  int addrlen = sizeof(address);
  int client_socket =
      accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
  if (client_socket < 0) {
    std::cerr << "Accept failed! Error: " << strerror(errno) << std::endl;
    close(server_fd);
    return -1;
  }

  // 7. 接收客户端消息
  char buffer[1024] = {0};
  read(client_socket, buffer, 1024); // 从客户端读取数据
  std::cout << "Message from client: " << buffer << std::endl;

  // 8. 发送响应
  if (strcmp(buffer, "getdatetime") == 0) {
    std::string currentTime = getCurrentDateTime();
    send(client_socket, currentTime.c_str(), currentTime.size(), 0);
  }

  // 9. 关闭 Socket
  close(client_socket);
  close(server_fd);
  return 0;
}

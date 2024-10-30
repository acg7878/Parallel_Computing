#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  // 1. 创建 Socket
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd == -1) {
    std::cerr << "Socket creation failed!" << std::endl;
    return -1;
  }

  // 2. 服务器地址配置
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(8888);
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // 连接到本地服务器

  // 3. 连接到服务器
  if (connect(client_fd, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    std::cerr << "Connection to server failed!" << std::endl;
    close(client_fd);
    return -1;
  }

  // 4. 发送请求
  const char *request = "getdatetime";
  send(client_fd, request, strlen(request), 0);

  // 5. 接收响应
  char buffer[1024] = {0};
  read(client_fd, buffer, 1024);
  std::cout << "Current DateTime from server: " << buffer << std::endl;

  // 6. 关闭 Socket
  close(client_fd);
  return 0;
}

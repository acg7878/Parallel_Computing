#ifndef CONSUMERCLIENT_H
#define CONSUMERCLIENT_H

#include <string>
class ConsumerClient {
public:
  ConsumerClient(const std::string &server_ip, int port);
  void start();

private:
  void connect_to_server(); // 连接服务器的函数
  void consume_data();

  std::string server_ip_;
  int port_;
  int sock_;
};

#endif // CONSUMERCLIENT_H

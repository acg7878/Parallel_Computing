#ifndef PRODUCERCLIENT_H
#define PRODUCERCLIENT_H

#include <string>
class ProducerClient {
public:
  ProducerClient(const std::string &server_ip, int port);
  void start();

private:
  void connect_to_server(); // 连接服务器的函数
  void produce_data();

  std::string server_ip_;
  int port_;
  int sock_;
};

#endif // PRODUCERCLIENT_H
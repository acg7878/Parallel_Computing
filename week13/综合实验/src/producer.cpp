#include "common.hpp"
#include <boost/asio.hpp>
#include <ctime>
#include <iostream>
#include <random>

int main() {
  try {
    boost::asio::io_context io_context;
    tcp::socket socket(io_context);
    tcp::endpoint endpoint(tcp::v4(), PRODUCER_PORT);

    socket.connect(endpoint);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(2000000000, std::numeric_limits<int>::max());

    for (int i = 1; i <= 1000000; ++i) {
      Message msg;
      msg.id = i;
      msg.timestamp = std::time(nullptr);
      msg.tag = "RandomNumber";

      // 使用生成的随机数
      msg.body = std::to_string(dis(gen));

      boost::asio::write(socket, boost::asio::buffer(msg.serialize()));
    }

    std::cout << "Producer finished generating numbers." << std::endl;
  } catch (const boost::system::system_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}

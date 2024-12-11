#ifndef COMMON_HPP
#define COMMON_HPP

#include <boost/asio.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using boost::asio::ip::tcp;

constexpr int PRODUCER_PORT = 12345;
constexpr int CONSUMER_PORT = 12346;
constexpr int BUFFER_SIZE = 1024;


inline bool isPrime(int64_t number) {
    if (number < 2) return false;
    for (int64_t i = 2; i * i <= number; ++i) {
        if (number % i == 0) return false;
    }
    return true;
}


// 消息结构体
struct Message {
  int id;                // 消息序号
  std::time_t timestamp; // 时间戳
  std::string tag;       // 消息Tag
  std::string body;      // 消息体

  // 序列化为字符串
  std::string serialize() const {
    return std::to_string(id) + "|" + std::to_string(timestamp) + "|" + tag +
           "|" + body + "\n";
  }

  // 从字符串反序列化
  static Message deserialize(const std::string &data) {
    Message msg;
    auto delim1 = data.find('|');
    auto delim2 = data.find('|', delim1 + 1);
    auto delim3 = data.find('|', delim2 + 1);
    msg.id = std::stoi(data.substr(0, delim1));
    msg.timestamp = std::stol(data.substr(delim1 + 1, delim2 - delim1 - 1));
    msg.tag = data.substr(delim2 + 1, delim3 - delim2 - 1);
    msg.body = data.substr(delim3 + 1);
    return msg;
  }
};

#endif // COMMON_HPP

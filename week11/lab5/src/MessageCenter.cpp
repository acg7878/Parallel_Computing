#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
// #include <vector>
#include <chrono>
#include <cstring>
#include <ctime>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

// 消息结构体
struct Message {
  int sequence;          // 消息序号
  std::string timestamp; // 时间戳
  std::string tag;       // 消息标签
  std::string body;      // 消息内容
};

// 消费者信息
struct ConsumerInfo {
  int socket; // 消费者的 socket 描述符
};

// 消息中心类
class MessageCenter {
private:
  std::unordered_map<int, ConsumerInfo> consumers; // 消费者列表
  std::queue<Message> messageQueue;                // 消息队列
  std::mutex queueMutex;                           // 队列锁
  std::mutex consumersMutex;                       // 消费者列表锁
  int messageSequence = 0;                         // 消息序号生成
  int serverSocket;       // 消息中心服务器 socket
  const int port = 12345; // 消息中心监听端口

  // 获取当前时间戳
  std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    return std::ctime(&currentTime);
  }

  // 处理生产者发送的消息
  void handleProducerMessage(const std::string &tag, const std::string &body) {
    Message message;
    {
      std::lock_guard<std::mutex> lock(queueMutex);
      message.sequence = ++messageSequence;
      message.timestamp = getCurrentTimestamp();
      message.tag = tag;
      message.body = body;
      messageQueue.push(message);
    }

    // 广播消息给所有消费者
    broadcastMessage(message);
  }

  // 广播消息
  void broadcastMessage(const Message &message) {
    std::lock_guard<std::mutex> lock(consumersMutex);
    for (const auto &[consumerId, info] : consumers) {
      std::string serializedMessage =
          "BROADCAST|" + std::to_string(message.sequence) + "|" +
          message.timestamp + "|" + message.tag + "|" + message.body + "\n";
      send(info.socket, serializedMessage.c_str(), serializedMessage.size(), 0);
    }
  }

  // 消息中心监听线程
  void acceptConnections() {
    struct sockaddr_in serverAddr;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
      perror("Socket creation failed");
      exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) <
        0) {
      perror("Bind failed");
      exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 10) < 0) {
      perror("Listen failed");
      exit(EXIT_FAILURE);
    }

    std::cout << "Message Center listening on port " << port << "...\n";

    while (true) {
      struct sockaddr_in clientAddr;
      socklen_t addrLen = sizeof(clientAddr);
      int clientSocket =
          accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);

      if (clientSocket < 0) {
        perror("Accept failed");
        continue;
      }

      // 创建线程处理客户端连接
      std::thread(&MessageCenter::handleClient, this, clientSocket).detach();
    }
  }

  // 处理客户端连接
  void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    read(clientSocket, buffer, 1024);
    std::string request(buffer);

    if (request.find("REGISTER") == 0) {
      // 消费者注册
      std::lock_guard<std::mutex> lock(consumersMutex);
      consumers[clientSocket] = {clientSocket};
      std::cout << "Consumer registered: " << clientSocket << "\n";
    } else if (request.find("MESSAGE") == 0) {
      // 生产者发送消息
      size_t posTag = request.find("|");
      size_t posBody = request.find("|", posTag + 1);
      std::string tag = request.substr(posTag + 1, posBody - posTag - 1);
      std::string body = request.substr(posBody + 1);
      handleProducerMessage(tag, body);
    } else {
      std::cout << "Unknown request: " << request << "\n";
    }
  }

public:
  // 启动消息中心
  void start() {
    std::thread(&MessageCenter::acceptConnections, this).detach();
  }

  // 停止消息中心
  void stop() { close(serverSocket); }
};

int main() {
  MessageCenter center;
  center.start();

  std::cout << "Press Enter to stop the server...\n";
  std::cin.get();

  center.stop();
  return 0;
}

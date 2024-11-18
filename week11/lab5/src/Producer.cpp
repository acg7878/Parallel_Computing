#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <thread>

// 生产者类
class Producer {
private:
    int serverSocket;                // 服务器 socket 描述符
    std::string serverAddress;       // 消息中心地址
    int serverPort;                  // 消息中心端口

    // 连接到消息中心
    bool connectToMessageCenter() {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            perror("Socket creation failed");
            return false;
        }

        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);

        if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0) {
            perror("Invalid address/Address not supported");
            return false;
        }

        if (connect(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            perror("Connection to server failed");
            return false;
        }

        std::cout << "Connected to Message Center at " << serverAddress << ":" << serverPort << "\n";
        return true;
    }

    // 断开连接
    void disconnect() {
        close(serverSocket);
        std::cout << "Disconnected from Message Center.\n";
    }

public:
    Producer(const std::string& address, int port)
        : serverAddress(address), serverPort(port) {}

    // 发送消息到消息中心
    void sendMessage(const std::string& tag, const std::string& body) {
        if (!connectToMessageCenter()) {
            return;
        }

        // 构造消息
        std::string message = "MESSAGE|" + tag + "|" + body + "\n";
        send(serverSocket, message.c_str(), message.size(), 0);

        std::cout << "Message sent: " << message;
        disconnect();
    }
};

// 获取当前时间戳
std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
    return std::string(buffer);
}

int main() {
    // 配置消息中心的地址和端口
    std::string serverAddress = "127.0.0.1"; // 本地测试
    int serverPort = 12345;

    Producer producer(serverAddress, serverPort);

    // 模拟消息发送
    for (int i = 1; i <= 5; ++i) {
        std::string tag = "INFO";
        std::string body = "Test message " + std::to_string(i) + " at " + getCurrentTimestamp();
        producer.sendMessage(tag, body);

        // 模拟生成消息的延迟
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}

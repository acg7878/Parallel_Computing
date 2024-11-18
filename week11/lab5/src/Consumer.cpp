#include <iostream>
#include <string>
#include <cstring>
//#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// 消费者类
class Consumer {
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

    // 注册为消费者
    void registerConsumer() {
        std::string registrationMessage = "REGISTER\n";
        send(serverSocket, registrationMessage.c_str(), registrationMessage.size(), 0);
        std::cout << "Registered as a Consumer.\n";
    }

    // 监听消息
    void listenForMessages() {
        char buffer[1024];
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(serverSocket, buffer, sizeof(buffer) - 1, 0);

            if (bytesRead <= 0) {
                std::cout << "Connection closed or error occurred.\n";
                break;
            }

            std::string message(buffer);
            std::cout << "Received Message: " << message << "\n";
        }
    }

public:
    Consumer(const std::string& address, int port)
        : serverAddress(address), serverPort(port) {}

    // 启动消费者
    void start() {
        if (!connectToMessageCenter()) {
            return;
        }

        registerConsumer();
        listenForMessages();

        // 断开连接
        close(serverSocket);
        std::cout << "Disconnected from Message Center.\n";
    }
};

int main() {
    // 配置消息中心的地址和端口
    std::string serverAddress = "127.0.0.1"; // 本地测试
    int serverPort = 12345;

    Consumer consumer(serverAddress, serverPort);
    consumer.start();

    return 0;
}

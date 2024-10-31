#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <thread>
#include <arpa/inet.h>

class Server {
public:
    Server(int port);
    void start();

private:
    int port;
    void handle_client(int client_socket);
    std::string process_request(const std::string& request);
};

#endif // SERVER_H

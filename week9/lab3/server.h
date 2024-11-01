#ifndef SERVER_H 
#define SERVER_H

#include <string>

#ifdef _WIN32  // Windows
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET socket_t;
#else  // Linux
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int socket_t;
#endif

class Server {
public:
    Server(int port);
    void start();

private:
    int port;
    void handle_client(socket_t client_socket);
    std::string process_request(const std::string& request);
};

#endif // SERVER_H

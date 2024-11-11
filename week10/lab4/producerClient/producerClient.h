#ifndef PRODUCERCLIENT_H
#define PRODUCERCLIENT_H

class producerClient {
public:
    producerClient(const char *ip, int port) : ip(ip), port(port) {}

private:
    const char *ip;
    int port;
};



#endif // PRODUCERCLIENT_H
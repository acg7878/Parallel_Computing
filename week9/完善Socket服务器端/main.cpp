#include "server.h"

int main() {
    Server server(3000);
    server.start();
    return 0;
}

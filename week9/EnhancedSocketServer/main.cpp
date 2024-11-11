//# 说明
//只写了win端，Linux端没空写了。
//了解了wstring这个玩意。
//由于cpp的标准库暂时没有线程池，这里使用了开源作者的库，即项目的thread_pool
//https://github.com/Razirp/ThreadPool/

#include "server.h"


int main() {
    Server server(3000);
    server.start();
    return 0;
}

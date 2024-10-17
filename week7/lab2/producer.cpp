#include "producer.h"

#include <iostream>
#include <limits>
#include <random>

#include "buffer.h"

void Producer::random_product(Buffer& buffer, int numProducers, int thread_id) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(2000000001,
                                         std::numeric_limits<int>::max());

    int num = 1000000 / numProducers; // 每个线程应该生产多少数字
    for (int i = 0; i < num; ++i) {
        int random_number = dist(gen);
        buffer.push(random_number);
    }
    std::cout << "线程 " << thread_id << " 生产完毕" << std::endl;
    if (thread_id == numProducers - 1) { // 最后一个线程设置完成标志
        buffer.setFinished();
    }
}

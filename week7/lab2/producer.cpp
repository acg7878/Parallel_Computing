#include "producer.h"

#include <iostream>
#include <limits>
#include <random>

#include "buffer.h"

void Producer::random_product(Buffer& buffer) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(2000000001,
                                         std::numeric_limits<int>::max());

    for (int i = 0; i < 1000000; ++i) {
        int random_number = dist(gen);
        buffer.push(random_number);
    }
    std::cout << "生产完毕" << std::endl;
    buffer.setFinished();
}

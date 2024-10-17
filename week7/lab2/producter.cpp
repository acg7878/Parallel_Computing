#include "producter.h"

#include <limits>
#include <random>

#include "buffer.h"

void Producter::random_product(Buffer& buffer) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, std::numeric_limits<int>::max());

    for (int i = 0; i < 1000000; ++i) {
        int random_number = dist(gen);
        buffer.push(random_number);
    }
}

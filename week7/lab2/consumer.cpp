#include "consumer.h"

#include <cmath>
#include <iostream>

#include "buffer.h"

bool printed = false;
void Consumer::isPrimeNumber(Buffer& buffer) {
    while (true) {
        int num = buffer.pop();
        if (buffer.getFinished()) { // 检查结束标志
            if (!printed) {
                printed = !printed;
                std::cout << "消费完毕" << std::endl;
            }
            break; // 结束循环
        }
        if (num <= 1 || (num != 2 && num % 2 == 0)) {
            continue; // 继续判断下一个数字
        }
        bool isPrime = true;
        for (int i = 3; i <= std::sqrt(num); i += 2) {
            if (num % i == 0) {
                isPrime = false;
                break;
            }
        }
    }
}
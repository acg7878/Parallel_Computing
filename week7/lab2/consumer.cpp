#include "consumer.h"

#include <cmath>

#include "buffer.h"

bool Consumer::isPrimeNumber(Buffer& buffer) {
    int num = buffer.pop();
    if (num <= 1 || (num != 2 && num % 2 == 0)) {
        return false;
    }
    for (int i = 3; i <= std::sqrt(num); i += 2) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}
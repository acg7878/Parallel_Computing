#include "buffer.h"

void Buffer::push(int num) {
    std::lock_guard<std::mutex> lock(mtx);
    numBuffer.push(num);
}

bool Buffer::empty() const {
    std::lock_guard<std::mutex> lock(mtx);
    return numBuffer.empty();
}

int Buffer::pop() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!numBuffer.empty()) {
        int num = numBuffer.front();
        numBuffer.pop();
        return num;
    }
    return -1; // 队列为空时返回 -1
}

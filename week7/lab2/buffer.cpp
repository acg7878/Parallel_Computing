#include "buffer.h"

#include <mutex>

void Buffer::push(int num) {
    std::lock_guard<std::mutex> lock(mtx);
    numBuffer.push(num);
    cv.notify_all();
}

bool Buffer::empty() const {
    std::lock_guard<std::mutex> lock(mtx);
    return numBuffer.empty();
}

int Buffer::pop() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this] { return !numBuffer.empty() || finished; });
    if (numBuffer.empty() && finished) return -1;
    int num = numBuffer.front();
    numBuffer.pop();
    return num;
}

void Buffer::setFinished() {
    std::lock_guard<std::mutex> lock(mtx);
    finished = true;
    cv.notify_all();
}
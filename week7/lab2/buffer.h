#ifndef BUFFER_H
#define BUFFER_H

#include <mutex>
#include <queue>
class Buffer {
public:
    void push(int num);
    int pop();
    bool empty() const;

private:
    mutable std::mutex mtx;
    std::queue<int> numBuffer;
};

#endif
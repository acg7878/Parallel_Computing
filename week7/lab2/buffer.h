#ifndef BUFFER_H
#define BUFFER_H

#include <condition_variable>
#include <mutex>
#include <queue>

class Buffer {
public:
    void push(int num);
    int pop();
    bool empty() const;
    void setFinished();
    bool getFinished();

private:
    mutable std::mutex mtx;
    std::queue<int> numBuffer;
    std::condition_variable cv;
    bool finished = false; // 结束标志
};

#endif
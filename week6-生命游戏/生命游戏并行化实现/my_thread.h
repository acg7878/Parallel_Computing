#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <barrier>
#include <thread>
#include <vector>

#include "game_of_life.h"

class ThreadManager {
   public:
    ThreadManager (GameOfLife& game, int numThreads);

    void run (int generations);  // 运行多个代数
   private:
    void worker (int startRow, int endRow, int threadId);  // 线程工作函数

    GameOfLife& game;
    int numThreads;
    std::barrier<> syncBarrier;        // 用于线程同步
    std::vector<std::thread> threads;  // 存储线程
};

#endif  // THREAD_H
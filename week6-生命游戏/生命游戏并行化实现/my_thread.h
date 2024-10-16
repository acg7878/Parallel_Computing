#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <thread>
#include <vector>

#include "game_of_life.h"

class ThreadManager {
public:
    ThreadManager(GameOfLife& board, int numThreads);
    void run();
    void threadTask(GameOfLife& board, int startRow, int endRow);

private:
    int numThreads;
    GameOfLife& board;
    std::vector<std::thread> threads;
};

#endif // THREAD_H
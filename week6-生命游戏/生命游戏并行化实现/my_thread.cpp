#include "my_thread.h"

#include <barrier> // C++20 标准中的屏障机制

// 构造函数：初始化线程数和屏障
ThreadManager::ThreadManager(GameOfLife& g, int nThreads)
    : board(g)
    , numThreads(nThreads)
    , syncPoint(nThreads, [this]() { board.swapGrids(); }) // 屏障的回调函数负责交换网格
{}

// 线程的工作函数，负责更新从 startRow 到 endRow 的网格部分
void ThreadManager::worker(int startRow, int endRow, int threadId, int generations) {
    for (int i = 0; i < generations; i++) {
        // 更新指定范围内的网格状态
        board.updateCellState(startRow, endRow);

        // 等待所有线程完成当前代的网格更新
        syncPoint.arrive_and_wait(); // 等待所有线程到达屏障

        // 网格交换由屏障的回调函数完成，只执行一次
        // 再次等待网格交换完成后，进入下一代
        syncPoint.arrive_and_wait();
    }
}

// 运行多代模拟，启动线程
void ThreadManager::run(int generations) {
    int chunkSize = board.getGrid().size() / numThreads;

    // 创建多个线程，每个线程负责一部分网格的更新
    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * chunkSize;
        int endRow = (i == numThreads - 1) ? board.getGrid().size() : startRow + chunkSize;

        threads.emplace_back(&ThreadManager::worker, this, startRow, endRow, i, generations);
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

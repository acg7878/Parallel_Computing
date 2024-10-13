#include "my_thread.h"

ThreadManager::ThreadManager (GameOfLife& g, int nThreads)
    : game (g), numThreads (nThreads), syncBarrier (nThreads) {}

void ThreadManager::worker (int startRow, int endRow, int threadId) {
    for (int generation = 0; generation < 100; ++generation) {
        game.updateCellState (startRow, endRow);

        // 等待所有线程完成本代计算
        syncBarrier.arrive_and_wait ();

        // 只有一个线程负责交换网格
        if (threadId == 0) {
            game.swapGrids ();
        }

        // 再次等待网格交换完成后继续下一代
        syncBarrier.arrive_and_wait ();
    }
}

void ThreadManager::run (int generations) {
    int chunkSize = game.getGrid ().size () / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        int startRow = i * chunkSize;
        int endRow = (i == numThreads - 1) ? game.getGrid ().size ()
                                           : startRow + chunkSize;
        threads.emplace_back (&ThreadManager::worker, this, startRow, endRow,
                              i);
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        if (thread.joinable ()) {
            thread.join ();
        }
    }
}

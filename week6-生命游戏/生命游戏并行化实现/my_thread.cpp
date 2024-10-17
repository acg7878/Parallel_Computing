#include "my_thread.h"

#include "game_of_life.h"

// 构造函数，初始化线程数量和GameOfLife对象的引用
ThreadManager::ThreadManager(GameOfLife& board, int numThreads)
    : board(board)
    , numThreads(numThreads) {
}

// 每个线程需要完成的任务：更新网格的一部分
void ThreadManager::threadTask(GameOfLife& board, int startRow, int endRow) {
    board.updateBoard(startRow, endRow); // 只更新网格的部分行数
}

void ThreadManager::run() {
    int rowsPerThread =
        board.getCurBoard().size() / numThreads; // 每个线程处理的行数
    int extraRows = board.getCurBoard().size() % numThreads; // 处理余下的行数

    int startRow = 0;
    // 创建多个线程，每个线程更新不同的网格区域
    for (int i = 0; i < numThreads; ++i) {
        int endRow = startRow + rowsPerThread;
        if (i == numThreads - 1) {
            endRow += extraRows; // 最后一个线程处理多余的行数
        }

        // 创建一个线程并启动，注意传递成员函数时的语法
        threads.emplace_back(&ThreadManager::threadTask, this, std::ref(board),
                             startRow, endRow);
        startRow = endRow; // 下一个线程的起始行数
    }

    // 等待所有线程完成
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join(); // 主线程等待子线程完成
        }
    }

    // 清空线程队列
    threads.clear();
}

#include <iostream>
#include <thread>

#include "game_of_life.h"
#include "my_thread.h"
#include "util.h"
int main() {
    int rows, cols, generation;
    int numThreads = std::thread::hardware_concurrency();
    std::cout << "输入行数、列数、迭代数：";
    std::cin >> rows >> cols >> generation;
    GameOfLife Board(rows, cols, generation);

    if (Board.initialize()) {
        return 1;
    }

    ThreadManager threadManager(Board, numThreads);
    for (int gen = 0; gen < generation; ++gen) {
        // 使用线程管理器更新网格
        threadManager.run();
        if ((gen + 1) % (generation / 10) == 0 || gen == generation - 1) {
            std::cout << "已完成 " << gen + 1 << " 代，共 " << generation
                      << " 代。" << std::endl;
        }
    }

    // 进行数据对比验证正确性
    if (compareBoards(Board.getCurBoard(), Board.getAnsBoard()))
        std::cout << "测试样例通过" << std::endl;
    else
        std::cout << "测试样例未通过" << std::endl;
    ;
    return 0;
}

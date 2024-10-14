#include <iostream>
#include <thread>

#include "game_of_life.h"
#include "my_thread.h"

int main () {
    int rows, cols, generation;
    int numThreads = std::thread::hardware_concurrency ();
    std::cout << "输入行数、列数、迭代数：";
    std::cin >> rows >> cols >> generation;
    GameOfLife board (rows, cols, generation);

    if (board.initialize ()) {
        std::cerr << "无法打开文件 " << std::endl;
        return 1;
    }
    // std::cout << "Initial Generation: " << std::endl;
    // game.printGrid ();

    ThreadManager manager (board, numThreads);
    manager.run (100);  // 运行100代

    // 进行数据对比验证正确性

    return 0;
}

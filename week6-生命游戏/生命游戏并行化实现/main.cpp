#include <iostream>
#include <thread>

#include "game_of_life.h"
#include "my_thread.h"

int main () {
    int rows, cols, generations;
    int numThreads = std::thread::hardware_concurrency ();
    std::cout << "输入行数、列数、迭代数：";
    std::cin >> rows >> cols >> generations;
    GameOfLife board (rows, cols, generations);

    if (board.initialize ()) {
        std::cerr << "无法打开文件 " << std::endl;
        return 1;
    }
    // std::cout << "Initial Generation: " << std::endl;
    // game.printGrid ();

    ThreadManager manager (board, numThreads);
    manager.run (generations);  // 运行100代

    // 进行数据对比验证正确性

    return 0;
}

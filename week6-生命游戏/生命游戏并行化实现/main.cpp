#include <iostream>
#include <thread>

#include "game_of_life.h"
#include "my_thread.h"

int main () {
    int rows, cols;
    int numThreads = std::thread::hardware_concurrency ();
    std::cin >> rows >> cols;
    GameOfLife game (rows, cols);
    game.initialize ();
    std::cout << "Initial Generation: " << std::endl;
    game.printGrid ();

    ThreadManager manager (game, numThreads);
    manager.run (100);  // 运行100代

    std::cout << "Final Generation: " << std::endl;
    game.printGrid ();

    return 0;
}

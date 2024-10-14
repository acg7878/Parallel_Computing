#include "game_of_life.h"
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    const int ROWS = 10000;
    const int COLS = 10000;
    const int ITERATIONS = 100;

    GameOfLife game(ROWS, COLS);

    try {
        game.loadFromFile(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error loading file: " << e.what() << std::endl;
        return 1;
    }

    int num_threads = std::thread::hardware_concurrency();
    std::cout << "Using " << num_threads << " threads." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; ++i) {
        if (i % 10 == 0) {
            std::cout << "Iteration " << i << std::endl;
        }
        game.step(num_threads);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Simulation completed in " << duration.count() << " ms." << std::endl;

    try {
        game.saveToFile(argv[2]);
        std::cout << "Final state saved to " << argv[2] << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving file: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
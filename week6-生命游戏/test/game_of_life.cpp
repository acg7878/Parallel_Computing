#include "game_of_life.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <stdexcept>
#include <mutex>
GameOfLife::GameOfLife(int r, int c) : rows(r), cols(c) {
    grid.resize(rows, std::vector<bool>(cols, false));
    next_grid.resize(rows, std::vector<bool>(cols, false));
}

void GameOfLife::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string line;
    int row = 0;
    while (std::getline(file, line) && row < rows) {
        for (int col = 0; col < cols && col < static_cast<int>(line.length()); ++col) {
            grid[row][col] = (line[col] == '1');
        }
        ++row;
    }
}

int GameOfLife::countNeighbors(int row, int col) const {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int r = (row + i + rows) % rows;
            int c = (col + j + cols) % cols;
            if (grid[r][c]) count++;
        }
    }
    return count;
}

void GameOfLife::updateCell(int row, int col) {
    int neighbors = countNeighbors(row, col);
    if (grid[row][col]) {
        next_grid[row][col] = (neighbors == 2 || neighbors == 3);
    } else {
        next_grid[row][col] = (neighbors == 3);
    }
}

void GameOfLife::updateRegion(int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < cols; ++j) {
            updateCell(i, j);
        }
    }
}

void GameOfLife::step(int num_threads) {
    std::vector<std::thread> threads;
    int rows_per_thread = rows / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int start_row = i * rows_per_thread;
        int end_row = (i == num_threads - 1) ? rows : (i + 1) * rows_per_thread;
        threads.emplace_back(&GameOfLife::updateRegion, this, start_row, end_row);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::lock_guard<std::mutex> lock(mtx);
    std::swap(grid, next_grid);
}

void GameOfLife::print() const {
    std::lock_guard<std::mutex> lock(mtx);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << (grid[i][j] ? "■ " : "□ ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void GameOfLife::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Unable to open file for writing: " + filename);
    }

    std::lock_guard<std::mutex> lock(mtx);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file << (grid[i][j] ? '1' : '0');
        }
        file << '\n';
    }
}
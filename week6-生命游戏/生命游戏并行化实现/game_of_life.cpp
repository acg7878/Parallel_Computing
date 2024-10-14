#include "game_of_life.h"

#include <iostream>
#include "util.h"

GameOfLife::GameOfLife (int r, int c, int n)
    : rows (r),
      cols (c),
      generations (n),
      grid (r, std::vector<int> (c, 0)),
      newGrid (r, std::vector<int> (c, 0)) {}

bool GameOfLife::initialize () {
    setCurrentPathToProjectRoot ();
    // printCurrentPath ();
    int fileIndex;
    std::cout << "请输入文件序号: ";
    std::cin >> fileIndex;
    // 选择数据、输入数据
    std::string initFilename =
        "test_data/" + std::to_string (fileIndex) + ".init.txt";
    if(!loadBoardFromFile(initFilename, grid, rows, cols)){
        return 1;
    }
    
    return 0;
}

void GameOfLife::printGrid () {
    for (const auto& row : grid) {
        for (int cell : row) {
            std::cout << (cell ? "O" : ".") << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int GameOfLife::countNeighbors (int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int nx = (x + i + rows) % rows;  // 考虑周期性边界
            int ny = (y + j + cols) % cols;
            count += grid[nx][ny];
        }
    }
    return count;
}

void GameOfLife::updateCellState (int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < cols; ++j) {
            int neighbors = countNeighbors (i, j);
            if (grid[i][j] == 1) {
                newGrid[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            } else {
                newGrid[i][j] = (neighbors == 3) ? 1 : 0;
            }
        }
    }
}

void GameOfLife::swapGrids () {
    grid.swap (newGrid);
}

const std::vector<std::vector<int>>& GameOfLife::getGrid () const {
    return grid;
}

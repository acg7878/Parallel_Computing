#include "game_of_life.h"

#include <iostream>
#include "util.h"

GameOfLife::GameOfLife(int r, int c, int n)
    : rows(r)
    , cols(c)
    , generation(n)
    , board(r, std::vector<int>(c, 0))
    , nextBoard(r, std::vector<int>(c, 0)) {
}

bool GameOfLife::initialize() {
    setCurrentPathToProjectRoot();
    // printCurrentPath ();
    int fileIndex;
    std::cout << "请输入文件序号: ";
    std::cin >> fileIndex;
    // 选择数据、输入数据
    std::string initFilename =
        "test_data/" + std::to_string(fileIndex) + ".init.txt";
    std::string endFilename =
        "test_data/" + std::to_string(fileIndex) + ".end.txt";
    if (!loadBoardFromFile(initFilename, board, rows, cols)) {
        std::cerr << "无法init打开文件 " << std::endl;
        return 1;
    }
    if (!loadBoardFromFile(endFilename, ansBoard, rows, cols)) {
        std::cerr << "无法end打开文件 " << std::endl;
        return 1;
    }
    
    return 0;
}

int GameOfLife::countNeighbors(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int nx = (x + i + rows) % rows; // 考虑周期性边界
            int ny = (y + j + cols) % cols;
            count += board[nx][ny];
        }
    }
    return count;
}

void GameOfLife::updateBoard(int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < cols; ++j) {
            int neighbors = countNeighbors(i, j);
            if (board[i][j] == 1) {
                nextBoard[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            } else {
                nextBoard[i][j] = (neighbors == 3) ? 1 : 0;
            }
        }
    }
}

void GameOfLife::swapBoard() {
    board.swap(nextBoard);
}

const std::vector<std::vector<int>>& GameOfLife::getCurBoard() const {
    return board;
}
const std::vector<std::vector<int>>& GameOfLife::getAnsBoard() const {
    return ansBoard;
}

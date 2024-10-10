#include <fstream>
#include <iostream>
#include <vector>
#include "Util.h"

int getAliveNumber(std::vector<std::vector<int>>& board,
                   int x,
                   int y,
                   int rows,
                   int cols) {
    // x,y为细胞坐标
    int aliveNumber = 0;
    int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                            {0, 1},   {1, -1}, {1, 0},  {1, 1}};
    // 左上、上、右上；左、右；左下、下、右下
    for (int i = 0; i < 8; i++) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];

        if (newX >= 0 && newX < rows && newY >= 0 && newY < cols) {
            aliveNumber += board[newX][newY];
        }
    }

    return aliveNumber;
}

std::vector<std::vector<int>>
nextGeneration(std::vector<std::vector<int>>& board, int rows, int cols) {
    std::vector<std::vector<int>> newboard(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int aliveNumber = getAliveNumber(board, i, j, rows, cols);
            if (aliveNumber == 3) {
                newboard[i][j] = 1;
            } else if (aliveNumber == 2) {
                newboard[i][j] = board[i][j];
            } else {
                newboard[i][j] = 0;
            }
        }
    }
    return newboard;
}

int main(int argc, char* argv[]) {
    int rows, cols, generation;
    std::vector<std::vector<int>> start_board;
    std::vector<std::vector<int>> end_board;
    int fileIndex;
    std::cout << "请输入文件序号: ";
    std::cin >> fileIndex;

    // 选择数据、输入数据
    std::string initFilename =
        "input_data/" + std::to_string(fileIndex) + ".init.txt";
    if (!loadBoardFromFile(initFilename, start_board, rows, cols,
                           generation)) {
        return 1;
    }
    // 计算下n代
    for (int i = 0; i < generation; i++) {
        start_board = nextGeneration(start_board, rows, cols);
    }
    std::string endFilename =
        "input_data/" + std::to_string(fileIndex) + ".end.txt";

    // 输入结果用例
    if (!loadBoardFromFile(endFilename, end_board, rows, cols, generation)) {
        return 1;
    }

    // 判断是否通过
    if (compareBoards(start_board, end_board)) {
        std::cout << "测试用例" << initFilename << "通过" << std::endl;
    } else {
        std::cout << "测试用例" << initFilename << "未通过" << std::endl;
    }
    return 0;
}

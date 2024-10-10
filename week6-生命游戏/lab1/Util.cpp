#include <fstream>
#include <iostream>
#include <vector>

// 读取文件内容到board中
bool loadBoardFromFile(const std::string& filename,
                       std::vector<std::vector<int>>& board,
                       int& rows,
                       int& cols,
                       int& generation) {
    std::ifstream inputFile;
    try {
        inputFile.open(filename.data());
        if (!inputFile) {
            throw std::runtime_error("无法打开文件: " + filename);
        }
        // 读取行数、列数和代数
        inputFile >> rows >> cols >> generation;
        // 调整 board 的大小并填充数据
        board.resize(rows, std::vector<int>(cols));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                inputFile >> board[i][j];
            }
        }
        inputFile.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool compareBoards(const std::vector<std::vector<int>>& board1,
                   const std::vector<std::vector<int>>& board2) {
    if (board1.size() != board2.size())
        return false;
    for (size_t i = 0; i < board1.size(); ++i) {
        if (board1[i] != board2[i])
            return false;
    }
    return true;
}
#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

// 声明读取文件的函数
bool loadBoardFromFile(const std::string& filename,
                       std::vector<std::vector<int>>& board,
                       int& rows,
                       int& cols,
                       int& generation);
bool compareBoards(const std::vector<std::vector<int>>& board1,
                   const std::vector<std::vector<int>>& board2);

#endif  // UTIL_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
// 读取文件内容到board中

void printCurrentPath () {
    std::filesystem::path currentPath = std::filesystem::current_path ();
    std::cout << "当前路径: " << currentPath << std::endl;
}
void setCurrentPathToProjectRoot () {
    // 设置当前路径为项目根目录（假设当前工作目录是 src 目录）
    std::filesystem::path projectRootPath =
        std::filesystem::current_path ().parent_path ();  // 上一级目录
    std::filesystem::current_path (
        projectRootPath);  // 将工作目录改为项目根目录
}
void saveBoardToFile (const std::vector<std::vector<int>> &board) {
    std::ofstream outputFile ("output.txt");  // 以覆盖模式打开文件

    if (!outputFile) {
        std::cerr << "无法打开文件 " << std::endl;
        return;
    }

    for (const auto &row : board) {
        for (const auto &cell : row) {
            outputFile << cell;  // 输出每个单元格，不添加空格和换行
        }
    }

    outputFile.close ();  // 关闭文件
}

bool compareBoards (const std::vector<std::vector<int>> &board1,
                    const std::vector<std::vector<int>> &board2) {
    saveBoardToFile (board1);
    if (board1.size () != board2.size ()) return false;
    for (size_t i = 0; i < board1.size (); ++i) {
        if (board1[i] != board2[i]) return false;
    }
    return true;
}
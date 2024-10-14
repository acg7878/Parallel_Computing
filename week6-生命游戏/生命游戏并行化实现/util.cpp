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

bool loadBoardFromFile(const std::string &filename, std::vector<std::vector<int>> &board, int &rows, int &cols) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }

    // 读取行数和列数
    rows = 0;
    cols = 0;

    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char cell : line) {
            if (cell == '0' || cell == '1') { // 仅处理0和1
                row.push_back(cell - '0'); // 将字符转换为整型
            }
        }
        if (cols == 0) {
            cols = row.size(); // 第一个读取的行作为列数
        }
        board.push_back(row);
        rows++;
    }

    file.close(); // 关闭文件
    return true;
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
#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <vector>

class GameOfLife {
public:
    GameOfLife(int rows, int cols, int generation);

    bool initialize();                          // 初始化网格
    void updateBoard(int startRow, int endRow); // 更新细胞状态
    int countNeighbors(int x, int y);           // 计算邻居数量

    const std::vector<std::vector<int>>& getCurBoard() const; // 获取当前网格
    const std::vector<std::vector<int>>& getAnsBoard() const; // 获取答案网格
    void swapBoard(); // 交换新旧网格

private:
    int rows;
    int cols;
    int generation;
    std::vector<std::vector<int>> board;     // 当前网格
    std::vector<std::vector<int>> nextBoard; // 下一代网格
    std::vector<std::vector<int>> ansBoard;  // 答案网格，用于对比
};

#endif // GAME_OF_LIFE_H

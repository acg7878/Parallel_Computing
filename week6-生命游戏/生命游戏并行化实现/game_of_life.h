#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <vector>

class GameOfLife {
public:
    GameOfLife (int rows, int cols, int generation);

    bool initialize ();                               // 初始化网格
    void printGrid ();                                // 打印当前网格
    void updateCellState (int startRow, int endRow);  // 更新细胞状态
    int countNeighbors (int x, int y);                // 计算邻居数量

    const std::vector<std::vector<int>>& getGrid () const;  // 获取当前网格
    void swapGrids ();  // 交换新旧网格

private:
    int rows;
    int cols;
    int generations;
    std::vector<std::vector<int>> grid;     // 当前网格
    std::vector<std::vector<int>> newGrid;  // 下一代网格
};

#endif  // GAME_OF_LIFE_H

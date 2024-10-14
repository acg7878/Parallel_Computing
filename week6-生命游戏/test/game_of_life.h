#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <vector>
#include <mutex>
#include <string>

class GameOfLife {
private:
    std::vector<std::vector<bool>> grid;
    std::vector<std::vector<bool>> next_grid;
    int rows, cols;
    mutable std::mutex mtx;  // 修改：将 mtx 声明为 mutable

    int countNeighbors(int row, int col) const;
    void updateCell(int row, int col);
    void updateRegion(int start_row, int end_row);

public:
    GameOfLife(int r, int c);
    void loadFromFile(const std::string& filename);
    void step(int num_threads);
    void print() const;
    void saveToFile(const std::string& filename) const;
};

#endif // GAME_OF_LIFE_H
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

const int SIZE = 1000;        // 棋盘大小
const int GENERATIONS = 1000; // 进化代数

// 初始化棋盘，随机生成0和1
void initializeBoard(std::vector<std::vector<int>> &board) {
  std::srand(std::time(nullptr));
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      board[i][j] = std::rand() % 2; // 随机生成0或1
    }
  }
}

// 计算某个细胞周围存活细胞的数量
int countAliveNeighbors(const std::vector<std::vector<int>> &board, int x,
                        int y) {
  int aliveCount = 0;
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      if (i == 0 && j == 0)
        continue; // 跳过自身
      int newX = x + i;
      int newY = y + j;
      // 判断是否越界
      if (newX >= 0 && newX < SIZE && newY >= 0 && newY < SIZE) {
        aliveCount += board[newX][newY];
      }
    }
  }
  return aliveCount;
}

// 进行一次进化
void evolve(std::vector<std::vector<int>> &board) {
  std::vector<std::vector<int>> newBoard =
      board; // 创建一个新棋盘来存储更新后的状态
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      int aliveNeighbors = countAliveNeighbors(board, i, j);
      if (aliveNeighbors == 3) {
        newBoard[i][j] = 1; // 周围有3个活细胞，当前细胞为生
      } else if (aliveNeighbors != 2) {
        newBoard[i][j] = 0; // 其他情况下，当前细胞为死
      }
    }
  }
  board = newBoard; // 更新棋盘
}

// 打印棋盘状态（可以选择在调试时使用）
void printBoard(const std::vector<std::vector<int>> &board) {
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      std::cout << (board[i][j] ? "O" : "."); // 使用O表示生，.表示死
    }
    std::cout << std::endl;
  }
}

int main() {
  std::vector<std::vector<int>> board(SIZE, std::vector<int>(SIZE));
  initializeBoard(board); // 初始化棋盘

  for (int generation = 0; generation < GENERATIONS; ++generation) {
    evolve(board); // 进行一次进化
    // printBoard(board);  // 打印当前棋盘状态（可以选择注释掉）
  }

  std::cout << "Game of Life completed after " << GENERATIONS << " generations."
            << std::endl;
  return 0;
}

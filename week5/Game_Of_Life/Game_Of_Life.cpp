#include <iostream>
#include <random>
#include <vector>

std::vector<std::vector<int>> generateBoard(int rows, int cols) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 1);
  std::vector<std::vector<int>> chessboard(rows, std::vector<int>(cols));
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      chessboard[i][j] = dis(gen);
    }
  }
  return chessboard;
}

int getAliveNumber(std::vector<std::vector<int>> &board, int x, int y, int rows,
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
nextGeneration(std::vector<std::vector<int>> &board, int rows, int cols) {
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

void printBoard(std::vector<std::vector<int>> board, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      std::cout << board[i][j] << " ";
    }
    std::cout << std::endl;
  }
}
int main(int argc, char *argv[]) {
  int rows = 1000;
  int cols = 1000;
  int generation = 1000;
  std::vector<std::vector<int>> board = generateBoard(rows, cols);
  for (int i = 0; i < generation; i++) {
    board = nextGeneration(board, rows, cols);
  }
  // printBoard(board, rows, cols);
  return 0;
}

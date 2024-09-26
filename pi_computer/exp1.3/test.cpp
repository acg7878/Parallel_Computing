#include <functional>
#include <iostream>
#include <limits>
#include <mutex>
#include <thread>

double sum = 0.0;

void addTask(int start, int step, int end, std::mutex &mtx) {
  double localSum = 0.0;
  for (int i = start; i < end; i += step) {
    localSum += 1.0 / (2 * i + 1);
  }
  std::lock_guard<std::mutex> grd(mtx);
  sum += localSum;
}

void subtractTask(int start, int step, int end, std::mutex &mtx) {
  double localSum = 0.0;
  for (int i = start; i < end; i += step) {
    localSum -= 1.0 / (2 * i + 1);
  }
  std::lock_guard<std::mutex> grd(mtx);
  sum += localSum;
}

int main() {
  std::mutex mtx;
  int maxIterations = 2000000000; // std::numeric_limits<int>::max();
  std::cout << std::numeric_limits<int>::max() << std::endl;
  std::thread addThread(addTask, 0, 2, maxIterations, std::ref(mtx));
  std::thread subtractThread(subtractTask, 1, 2, maxIterations, std::ref(mtx));

  // 等待两个线程完成
  addThread.join();
  subtractThread.join();

  // 乘以 4 得到 pi 的值
  double pi = 4 * sum;
  std::cout << "Pi = " << pi << std::endl;

  return 0;
}

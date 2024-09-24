#include <algorithm>
#include <atomic>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <thread>
#include <vector>

void computer_pi_part(int start, int end, std::atomic<double> &pi_part) {
  double local_sum = 0.0;
  bool positive = (start % 2 == 0);
  for (int i = start; i < end; i++) {
    if (positive) {
      local_sum += 1.0 / (2.0 * i + 1.0);
    } else {
      local_sum -= 1.0 / (2.0 * i + 1.0);
    }
    positive = !positive;
  }
  pi_part = pi_part + local_sum;
}

int main(int argc, char *argv[]) {
  int IntMax = std::numeric_limits<int>::max();
  std::atomic<double> pi(0.0);
  int num_thread = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  int size = IntMax / num_thread;

  for (int i = 0; i < num_thread; i++) {
    int start = i * size;
    int end = (i == num_thread - 1) ? IntMax : start + size;
    threads.emplace_back(computer_pi_part, start, end, std::ref(pi));
  }
  for (auto &t : threads) {
    t.join();
  }
  double PI = pi * 4.0;
  std::cout << PI << std::endl;
  return 0;
}

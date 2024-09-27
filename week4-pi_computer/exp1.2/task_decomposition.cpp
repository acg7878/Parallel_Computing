#include <iostream>
#include <atomic>
#include <limits>
#include <vector>
#include <thread>
void task_add(int start, int end, std::atomic<double> &pi_part) {
  double local_sum = 0.0;
  for (int i = start; i < end; i += 2) {
    local_sum += 1.0 / (2.0 * i + 1.0);
  }
  pi_part = pi_part + local_sum;
}

void task_sub(int start, int end, std::atomic<double> &pi_part) {
  double local_sum = 0.0;
  for (int i = start; i < end; i += 2) {
    local_sum -= 1.0 / (2.0 * i + 1.0);
  }
  pi_part = pi_part + local_sum;
}

int main(int argc, char *argv[]) {
  std::atomic<double> pi(0.0);
  int num_threads = 2;
  int IntMax = 2000000000;
  //这里使用std::numeric_limits<unsigned int>::max()会无响应，原因未知
  std::vector<std::thread> threads;
  threads.emplace_back(task_add,0,IntMax,std::ref(pi));
  threads.emplace_back(task_sub,1,IntMax,std::ref(pi));
  for (auto &t : threads) {
    t.join();
  }
  double PI = pi * 4.0;
  std::cout << PI << std::endl;
  return 0;
}

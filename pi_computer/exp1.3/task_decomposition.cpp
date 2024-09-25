#include <iostream>

void task_add(int start, int end, std::atomic<double> &pi_part) {
  double local_sum = 0.0;
  for (int i = 0; i < end; i += 2) {
    local_sum += 1.0 / (2.0 * i + 1.0);
  }
  pi_part = pi_part + local_sum;
}

void task_sub(int start, int end, std::atomic<double> &pi_part) {
  double local_sum = 0.0;
  for (int i = 1; i < end; i += 2) {
    local_sum -= 1.0 / (2.0 * i + 1.0);
  }
  pi_part = pi_part + local_sum;
}

int main(int argc, char *argv[]) { return 0; }

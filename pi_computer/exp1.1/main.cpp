#include <iomanip>
#include <iostream>
#include <limits>

int main(int argc, char *argv[]) {
  int IntMax = std::numeric_limits<int>::max();
  double pi = 0.0;
  bool positive = true;
  for (int i = 0; i < IntMax; i++) {
    if (positive) {
      pi += 1.0 / (2.0 * i + 1.0);
    } else {
      pi -= 1.0 / (2.0 * i + 1.0);
    }
    positive = !positive;
  }
  pi *= 4.0;
  std::cout << std::setprecision(15) << pi << std::endl;
  return 0;
}

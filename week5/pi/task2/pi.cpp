#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <mutex>
#include <ratio>
#include <thread>
#include <vector>

std::mutex mtx;
void computerPi(int start, int end, double& pi) {
    auto start_time = std::chrono::high_resolution_clock::now();
    double localSum = 0.0;
    bool positive = true;
    for (int i = start; i < end; i++) {
        if (positive)
            localSum += 1.0 / (2.0 * i - 1.0);
        else
            localSum -= 1.0 / (2.0 * i - 1.0);
        positive = !positive;
    }
    std::lock_guard<std::mutex> grd(mtx);
    pi += localSum;
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_time =
        end_time - start_time;
    std::cout << "耗时：" << duration_time.count() << "ms" << std::endl;
}

int main(int argc, char* argv[]) {
    int IntMax = std::numeric_limits<int>::max();
    double pi = 0.0;
    std::vector<std::thread> threads;
    threads.emplace_back(computerPi, 1, IntMax / 2, std::ref(pi));
    threads.emplace_back(computerPi, IntMax / 2, IntMax, std::ref(pi));
    for (auto& t : threads) {
        t.join();
    }
    pi *= 4.0;
    std::cout << std::setprecision(15) << pi << std::endl;
    return 0;
}

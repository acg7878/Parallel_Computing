#include <atomic>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <thread>
#include <vector>

void computer_pi_part(int start, int end, std::atomic<double>& pi_part) {
    auto start_time = std::chrono::high_resolution_clock::now();
    double local_sum = 0.0;
    bool positive = (start % 2 == 0);
    for (int i = start; i < end; i++) {
        if (positive)
            local_sum += 1.0 / (2.0 * i + 1.0);
        else
            local_sum -= 1.0 / (2.0 * i + 1.0);
        positive = !positive;
    }
    pi_part = pi_part + local_sum;
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);
    // std::chrono::duration<double, std::milli> duration_time =
    //     end_time - start_time;
    std::cout << "执行时间: " << duration_time.count() << " 毫秒" << std::endl;
}

int main(int argc, char* argv[]) {
    int IntMax = std::numeric_limits<int>::max();
    std::atomic<double> pi(0.0);
    int num_threads = 2;
    std::vector<std::thread> threads;
    int size = IntMax / num_threads;
    for (int i = 0; i < num_threads; i++) {
        int start = i * size;
        int end = (i == num_threads - 1) ? IntMax : start + size;
        threads.emplace_back(computer_pi_part, start, end, std::ref(pi));
    }
    for (auto& t : threads) {
        t.join();
    }
    double PI = pi * 4.0;
    std::cout << std::setprecision(15) << PI << std::endl;
    return 0;
}

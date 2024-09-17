#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

int main() {
    std::vector<int> arr;
    std::mutex mtx;
    std::thread t1([&] {
        for (int i = 0; i < 10; i++) {
            mtx.lock();   //不上锁会产生产生data-race
            arr.push_back(1);
            mtx.unlock();
        }
    });
    std::thread t2([&] {
        for (int i = 0; i < 10; i++) {
            mtx.lock();
            arr.push_back(2);
            mtx.unlock();
        }
    });
    t1.join();
    t2.join();

    for(int num : arr){
      std::cout << num << " ";
    }
    std::cout << std::endl;
    return 0;
}


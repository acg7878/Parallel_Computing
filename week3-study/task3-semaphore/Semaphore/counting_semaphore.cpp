#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>
#include <mutex> 

std::counting_semaphore sem(4); //可用资源为4
std::mutex cout_mutex; 

void worker(int id) {
    sem.acquire();
    {
        std::lock_guard<std::mutex> guard(cout_mutex); 
        //使用锁来防止cout输出混乱
        std::cout << "Thread " << id << " is working.\n";
    }//lock_guard只在作用域中生效，通过创建花括号将锁限制在一定范围
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> guard(cout_mutex); 
        std::cout << "Thread " << id << " is done.\n";
    }
    sem.release();
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(worker, i);
    }
    for (auto& t : threads) {
        t.join();
    }
    return 0;
}


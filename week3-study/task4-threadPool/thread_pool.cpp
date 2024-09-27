#include <boost/asio.hpp>
#include <boost/asio/dispatch.hpp>
#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>
std::mutex cout_mutex;

void my_task(int index) {
    std::lock_guard<std::mutex> lock(cout_mutex); // 确保线程安全输出
    std::cout << "task: " << index << " is running" << std::endl;
   // std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main(int argc, char *argv[]) {
    boost::asio::thread_pool pool(4);
    for (int i = 0; i < 10; i++) {
        boost::asio::dispatch(pool, [i] {
            my_task(i);
        });
    }
    pool.join();
    std::cout << "All tasks are over." << std::endl;
    return 0;
}

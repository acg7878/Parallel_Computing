#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

int main() {
  std::condition_variable cv;
  std::mutex mtx;
  bool ready = false;

  std::thread t1([&] {
    std::unique_lock lck(mtx);
    cv.wait(lck, [&] { return ready; });
    // cv.wait(lck,expr)的形式，其中 expr 是个
    // lambda 表达式，只有其返回值为 true 时
    // 才会真正唤醒，否则继续等待。
    std::cout << "t1 is awake" << std::endl;
  });

  std::cout << "notifying not ready" << std::endl;
  cv.notify_one(); // ready = false

  ready = true;
  std::cout << "notifying ready" << std::endl;
  cv.notify_one(); // ready = true

  t1.join();

  return 0;
}

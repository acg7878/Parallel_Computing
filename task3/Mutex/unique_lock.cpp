#include <iostream>
#include <mutex>
#include <ostream>
#include <thread>
#include <vector>
int main (int argc, char *argv[]) {
  std::vector<int> arr;
  std::mutex mtx;
  std::thread t1([&] {
    for(int i=0;i<10;i++){
      std::unique_lock grd(mtx);
      arr.push_back(1);
    }
  });
  std::thread t2([&] {
    for(int i=0;i<10;i++){
      std::unique_lock grd(mtx);
      arr.push_back(2);
      grd.unlock();
      std::cout << "解锁！！" << std::endl;
      grd.lock();
    }
  });

  t1.join();
  t2.join();
  for(int num:arr){
    std::cout << num << " ";
  }
  std::cout << std::endl;
  return 0;
}

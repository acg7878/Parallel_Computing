#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main (int argc, char *argv[]) {
  std::vector<int> arr;
  std::mutex mtx;
  std::thread t1([&] {
    for(int i=0;i<10;i++){
      std::lock_guard grd(mtx);
      arr.push_back(1);
    }
  });
  
  std::thread t2([&] {
    for(int i=0;i<10;i++){
      std::lock_guard grd(mtx);
      arr.push_back(2);
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

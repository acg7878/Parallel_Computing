#include <chrono>
#include <ostream>
#include <thread>
#include <iostream>

void test(){
  for(int i=0;i<10;i++){
    std::cout << "I'm a thread!!!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
  std::cout << "I'm over." << std::endl;
}

void interact(){
  std::string name;
  std::cin >> name;
  std::cout << "Hi," << name << std::endl;
}



int main (int argc, char *argv[]) {
  std::thread thread1(test);
  interact();
  thread1.join();
  return 0;
}

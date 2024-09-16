#include <iostream>
#include <tchar.h>
#include <windows.h>

// 线程函数
DWORD WINAPI ThreadFunction(LPVOID lpParam) {
  int threadNum = *(int *)lpParam;
  std::cout << "Thread " << threadNum << " started.\n";

  // 模拟工作
  for (int i = 0; i < 10; ++i) {
    std::cout << "Thread " << threadNum << " is working...\n";
    Sleep(500); // 模拟工作负载
  }

  std::cout << "Thread " << threadNum << " finished.\n";
  return 0;
}

int main() {
  HANDLE hThread1, hThread2;
  DWORD threadId1, threadId2;
  int threadNum1 = 1;
  int threadNum2 = 2;

  // 创建两个线程
  hThread1 = CreateThread(NULL,           // 默认安全属性
                          0,              // 默认栈大小
                          ThreadFunction, // 线程函数
                          &threadNum1,    // 传递给线程函数的参数
                          0,              // 默认创建标志
                          &threadId1);    // 返回线程 ID

  hThread2 = CreateThread(NULL,           // 默认安全属性
                          0,              // 默认栈大小
                          ThreadFunction, // 线程函数
                          &threadNum2,    // 传递给线程函数的参数
                          0,              // 默认创建标志
                          &threadId2);    // 返回线程 ID

  if (hThread1 == NULL || hThread2 == NULL) {
    std::cerr << "Failed to create threads. Error code: " << GetLastError()
              << std::endl;
    return 1;
  }

  // 设置线程优先级
  if (!SetThreadPriority(hThread1, THREAD_PRIORITY_ABOVE_NORMAL)) {
    std::cerr << "Failed to set priority for thread 1. Error code: "
              << GetLastError() << std::endl;
  } else {
    std::cout << "Priority for thread 1 set to ABOVE_NORMAL.\n";
  }

  if (!SetThreadPriority(hThread2, THREAD_PRIORITY_BELOW_NORMAL)) {
    std::cerr << "Failed to set priority for thread 2. Error code: "
              << GetLastError() << std::endl;
  } else {
    std::cout << "Priority for thread 2 set to BELOW_NORMAL.\n";
  }

  // 等待线程结束
  WaitForSingleObject(hThread1, INFINITE);
  WaitForSingleObject(hThread2, INFINITE);

  // 关闭线程句柄
  CloseHandle(hThread1);
  CloseHandle(hThread2);

  return 0;
}

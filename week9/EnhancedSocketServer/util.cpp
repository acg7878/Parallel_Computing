#include "util.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::wstring get_current_time() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm local_time = *std::localtime(&now_time);

  std::wostringstream oss;  // 使用宽字符流
  oss << std::put_time(&local_time, L"%Y%m%d%H%M%S");  // 使用宽字符格式
  return oss.str();  // 返回宽字符串
}

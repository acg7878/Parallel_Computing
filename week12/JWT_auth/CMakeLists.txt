cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(JWT_auth)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 curl 库
find_package(CURL REQUIRED)

# 查找 OpenSSL 库（用于 MD5）
find_package(OpenSSL REQUIRED)

# 添加 nlohmann-json 的头文件路径
include_directories("/opt/homebrew/include")

# 添加源文件
add_executable(JWT_auth main.cpp)

# 链接库
target_link_libraries(JWT_auth PRIVATE CURL::libcurl OpenSSL::Crypto)

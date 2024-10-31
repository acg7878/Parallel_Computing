cmake_minimum_required(VERSION 3.10)

# 项目名称
project(SocketServer)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找源文件
set(SOURCES
    main.cpp
    server.cpp
    utils.cpp
)

# 添加可执行文件
add_executable(SocketServer ${SOURCES})

# 设置线程库
find_package(Threads REQUIRED)
target_link_libraries(SocketServer Threads::Threads)

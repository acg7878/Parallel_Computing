cmake_minimum_required(VERSION 3.10)
project(SocketServer)

set(CMAKE_CXX_STANDARD 20)

# 根据操作系统选择对应的服务器文件
if(WIN32)
    set(SERVER_SRC server_windows.cpp)  # Windows 的 socket 实现
    # 查找并链接 Winsock 库
    find_library(WS2_32_LIBRARY ws2_32)
    set(LIBRARIES ${WS2_32_LIBRARY})
else()
    set(SERVER_SRC server_linux.cpp)    # Linux 的 socket 实现
    set(LIBRARIES "")
endif()

# 添加可执行文件
add_executable(SocketServer main.cpp ${SERVER_SRC} util.cpp thread_pool.cpp worker_thread.cpp)

# 链接库
target_link_libraries(SocketServer ${LIBRARIES})

# 设置项目名称
cmake_minimum_required(VERSION 3.10)
project(lab6_2)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

# 查找并添加 libcurl 库
find_package(CURL REQUIRED)

# 指定源文件
set(SOURCES
    main.cpp
)

# 添加可执行文件
add_executable(lab6_2 ${SOURCES})

# 链接 curl 库
target_link_libraries(lab6_2 CURL::libcurl)

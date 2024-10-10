# 最低CMake版本要求
cmake_minimum_required(VERSION 3.10)

# 定义项目名称
project(GameOfLife VERSION 1.0)

# 添加源文件
set(SOURCES
    GameOfLife.cpp
    Util.cpp
)

# 生成可执行文件
add_executable(GameOfLife ${SOURCES})


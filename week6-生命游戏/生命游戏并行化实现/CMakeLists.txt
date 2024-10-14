cmake_minimum_required(VERSION 3.10)
project(GameOfLife)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Add the executable
add_executable(GameOfLife main.cpp game_of_life.cpp my_thread.cpp util.cpp)


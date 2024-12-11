cmake_minimum_required(VERSION 3.10)
project(ProducerConsumerQueue)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()
# Boost库的查找和链接
find_package(Boost REQUIRED COMPONENTS system)

# 添加公共头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 定义生产者可执行文件
add_executable(producer src/producer.cpp)
target_link_libraries(producer Boost::system)

# 定义消息中心可执行文件
add_executable(message_center src/message_center.cpp)
target_link_libraries(message_center Boost::system pthread)

# 定义消费者可执行文件
add_executable(consumer src/consumer.cpp)
target_link_libraries(consumer Boost::system)


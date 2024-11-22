# 设置最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目信息
project(lab6.3 VERSION 1.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 libcurl
find_package(CURL REQUIRED)

# 添加源文件
set(SOURCES
    main.cpp
)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接 libcurl 库
target_link_libraries(${PROJECT_NAME} CURL::libcurl)

# 显示构建的详细信息
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "CURL Library: ${CURL_LIBRARIES}")

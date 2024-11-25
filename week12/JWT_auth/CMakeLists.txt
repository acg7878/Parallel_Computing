cmake_minimum_required(VERSION 3.10)
project(JWT_Auth_Project)

set(CMAKE_CXX_STANDARD 17)

# 查找必要的库
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)

# 添加头文件路径
include_directories(include) # 项目内部头文件

# 确保 Homebrew 安装的依赖被找到（Apple Silicon 或 Intel 环境）
include_directories(/opt/homebrew/include)  # For Apple Silicon
include_directories(/usr/local/include)    # For Intel-based Macs

# 添加源文件
add_executable(JWT_Auth_Project
        src/main.cpp
        src/jwt_utils.cpp
        src/http_client.cpp
)

# 链接必要的库
target_link_libraries(JWT_Auth_Project
        PRIVATE
        CURL::libcurl        # libcurl 库
        OpenSSL::SSL         # OpenSSL 的 SSL 库
        OpenSSL::Crypto      # OpenSSL 的 Crypto 库
        nlohmann_json::nlohmann_json # JSON 库
)

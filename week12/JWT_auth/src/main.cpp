#include "jwt_utils.hpp"
#include <iostream>
#include <string>

int main() {
    std::string username = "202226910925";
    // 接口 1
    std::string jwtToken = getToken(username);
    std::cout << "JWT Token: " << jwtToken << std::endl;
    // 接口 2
    std::cout << authTest(username,jwtToken) << std::endl;
    // 接口 3
    std::cout << postTest3(username,jwtToken) << std::endl;
    // 接口 4
    std::cout << postTest4(username,jwtToken) << std::endl;
    return 0;
}

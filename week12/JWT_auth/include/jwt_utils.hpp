#ifndef JWT_UTILS_HPP
#define JWT_UTILS_HPP

#include <string>

// JWT 相关函数
std::string generateJWT(const std::string &username, const std::string &secret);
bool verifyJWT(const std::string &jwtToken, const std::string &secret);

// HTTP 请求相关函数
std::string getToken(const std::string &username);
std::string authTest(const std::string &username, const std::string &jwtToken);
std::string postTest3(const std::string &username, const std::string &jwtToken);
std::string postTest4(const std::string &username, const std::string &jwtToken);


// MD5 工具函数
std::string computeMD5(const std::string &input);

#endif

#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <map>
#include <string>

std::string postJson(const std::string &url, const std::string &jsonBody, const std::string &jwtToken = "");
std::string postForm(const std::string &url, const std::string &userName, const std::string &passWord, const std::string &jwtToken = "");
std::string httpGET(const std::string &url, const std::map<std::string, std::string> &headers = {});
#endif // HTTP_CLIENT_HPP

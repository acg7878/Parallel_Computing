#include "jwt_utils.hpp"
#include "constants.hpp"
#include <openssl/hmac.h>
#include <openssl/md5.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <ctime>
#include <http_client.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>


// MD5 工具函数
std::string computeMD5(const std::string &input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char *>(input.c_str()), input.size(), digest);

    std::ostringstream md5String;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        md5String << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }
    return md5String.str();
}

// HTTP 回调函数
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

// 接口1
std::string getToken(const std::string &username) {
    const std::string password = computeMD5(username); // 计算用户名的 MD5 作为密码

    // 构造 JSON 请求体
    nlohmann::json requestBody = {
        {"userName", username},
        {"passWord", password}
    };
    std::string jsonBody = requestBody.dump();
    std::string response = postJson(TEST_URL_1, jsonBody);
    try {
        auto jsonResponse = nlohmann::json::parse(response);
        if (jsonResponse.contains("token")) {
            return jsonResponse["token"];
        } else {
            std::cerr << "Token not found in response: " << response << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
    }
    return "";
}

// 接口 2 实现
std::string authTest(const std::string &username, const std::string &jwtToken) {
    // 设置请求头
    std::map<std::string, std::string> headers;
    headers["Authorization"] = "Bearer " + jwtToken;
    std::string response = httpGET(TEST_URL_2+username, headers);
    // 检查响应是否为空
    if (response.empty()) {
        std::cerr << "authTest: Received an empty response." << std::endl;
        return "";
    }
    return response;
}


// 接口 3 实现
std::string postTest3(const std::string &username, const std::string &jwtToken) {
    std::string password =  computeMD5(username);
    nlohmann::json jsonBody = {
        {"userName", username},
        {"passWord", password}
    };
    std::string response = postJson(TEST_URL_3, jsonBody.dump(), jwtToken);
    if (response.empty()) {
        std::cerr << "postWithToken: Received an empty response." << std::endl;
        return "";
    }
    return response;
}


// 接口 4 实现
std::string postTest4(const std::string &username, const std::string &jwtToken) {
    const std::string password = computeMD5(username); // 使用用户名生成密码的 MD5
    std::string response = postForm(TEST_URL_4, username, password, jwtToken);
    if (response.empty()) {
        std::cerr << "postTest4: Received an empty response from the server." << std::endl;
        return "";
    }
    return response;
}

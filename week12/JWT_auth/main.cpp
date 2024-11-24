#include <curl/curl.h>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <openssl/md5.h>
#include <sstream>
#include <string>

// 回调函数，用于获取响应数据
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t total_size = size * nmemb;
  std::string *response = (std::string *)userp;
  response->append((char *)contents, total_size);
  return total_size;
}

// 计算 MD5 值并返回十六进制小写字符串
std::string md5(const std::string &input) {
  unsigned char digest[MD5_DIGEST_LENGTH];
  MD5_CTX ctx;
  MD5_Init(&ctx);
  MD5_Update(&ctx, input.c_str(), input.size());
  MD5_Final(digest, &ctx);

  std::stringstream ss;
  for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    ss << std::setw(2) << std::setfill('0') << std::hex << (int)digest[i];
  }
  return ss.str();
}

nlohmann::json getJWTtoken(const std::string &username) {
  nlohmann::json response;
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();

  // 将密码设置为 username 的 MD5 值（小写）
  std::string password = md5(username); // 使用 md5 函数

  // 创建 JSON 请求体
  nlohmann::json j;
  j["username"] = username;
  j["password"] = password;
  std::string data = j.dump(); // 转换为字符串

  // 保存响应数据
  std::string response_data;

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://47.115.44.145:7000/api/auth");

    // 设置请求头 Content-Type 为 application/json
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // 设置请求体数据
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

    // 设置回调函数来接收响应数据
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    // 执行请求
    res = curl_easy_perform(curl);

    // 检查请求是否成功
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    } else {
      // 如果请求成功，解析响应数据
      try {
        response = nlohmann::json::parse(response_data);
      } catch (const std::exception &e) {
        std::cerr << "Failed to parse response: " << e.what() << std::endl;
      }
    }

    // 清理头部
    curl_slist_free_all(headers);
    // 清理 curl
    curl_easy_cleanup(curl);
  }

  return response; // 返回 JSON 响应
}

nlohmann::json JWTTokenTest(const std::string &userName,
                            const std::string &bearerToken) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  nlohmann::json response;

  curl = curl_easy_init();
  if (curl) {
    // 设置目标URL
    std::string url =
        "http://47.115.44.145:7000/api/auth/authtest?userName=" + userName;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // 设置 Authorization 头部，Bearer token
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(
        headers, ("Authorization: Bearer " + bearerToken).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // 设置写回调函数以接收服务器的响应
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    // 执行请求
    res = curl_easy_perform(curl);

    // 检查请求是否成功
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    } else {
      // 打印返回的原始数据
      // std::cout << "Response: " << readBuffer << std::endl;

      // 解析JSON响应
      try {
        response = nlohmann::json::parse(readBuffer);
      } catch (const nlohmann::json::exception &e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
      }
    }

    // 清理curl
    curl_easy_cleanup(curl);
  }
  return response;
}

std::string postTest4(const std::string &userName, const std::string &passWord,
                      const std::string &jwtToken) { // 新增 jwtToken 参数
  CURL *curl;
  CURLcode res;
  std::string response;

  curl = curl_easy_init();
  if (curl) {
    nlohmann::json jsonData;
    jsonData["userName"] = userName;
    jsonData["passWord"] = passWord;

    // 设置 URL 和请求头
    curl_easy_setopt(curl, CURLOPT_URL,
                     "http://47.115.44.145:7000/api/Test/test4");

    // 设置 Content-Type 为 application/json
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // 添加 Authorization 头部，使用 Bearer token
    std::string authHeader = "Authorization: Bearer " + jwtToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::string jsonDataStr = jsonData.dump();
    // 设置 POST 数据（JSON 字符串）
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonDataStr.c_str());

    // 设置回调函数来接收响应
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // 执行请求
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    }

    curl_slist_free_all(headers); // 清理请求头
    curl_easy_cleanup(curl);      // 清理 curl
  }
  return response;
}

std::string postTest5(const std::string &userName, const std::string &passWord,
                      const std::string &jwtToken) { // 新增 jwtToken 参数
  CURL *curl;
  CURLcode res;
  std::string response;

  curl = curl_easy_init();
  if (curl) {
    // 创建 mime 句柄
    curl_mime *form = curl_mime_init(curl);
    curl_mimepart *field = NULL;

    // 添加 userName 字段
    field = curl_mime_addpart(form);
    curl_mime_name(field, "userName");
    curl_mime_data(field, userName.c_str(), CURL_ZERO_TERMINATED);

    // 添加 passWord 字段
    field = curl_mime_addpart(form);
    curl_mime_name(field, "passWord");
    curl_mime_data(field, passWord.c_str(), CURL_ZERO_TERMINATED);

    // 设置 URL
    curl_easy_setopt(curl, CURLOPT_URL,
                     "http://47.115.44.145:7000/api/Test/test5");

    // 设置 mime 数据
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

    // 设置 JWT Token 的 Authorization 头部
    struct curl_slist *headers = nullptr;
    std::string authHeader = "Authorization: Bearer " + jwtToken;
    headers = curl_slist_append(headers, authHeader.c_str());

    // 添加请求头
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // 设置回调函数来接收响应
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // 执行请求
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    }

    // 清理
    curl_slist_free_all(headers); // 清理请求头
    curl_mime_free(form);         // 清理 mime 句柄
    curl_easy_cleanup(curl);      // 清理 curl 句柄
  }
  return response;
}

int main() {
  std::string username; //= "202226910925";
  std::cout << "输入12位学号：";
  std::cin >> username;
  nlohmann::json token = getJWTtoken(username);
  std::cout << "接口1:\n " << token.dump(4) << std::endl;
  std::cout << "接口2:\n " << JWTTokenTest(username, token["token"])
            << std::endl;
  std::cout << "接口4:\n " << postTest4(username, "123456", "123")
            << std::endl;
  std::cout << "接口5:\n " << postTest5(username, "123456", token["token"])
            << std::endl;

  return 0;
}

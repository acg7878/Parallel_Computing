#include <curl/curl.h>
#include <iostream>
#include <string>

// 写入响应数据的回调函数
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

// GET 请求，不带参数，返回固定字符串
std::string getTest1() {
  CURL *curl;
  CURLcode res;
  std::string response;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL,
                     "http://47.115.44.145:7000/api/Test/test1");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    }

    curl_easy_cleanup(curl);
  }
  return response;
}

std::string getTest2() {
  CURL *curl;
  CURLcode res;
  std::string response;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL,
                     "http://47.115.44.145:7000/api/Test/test2");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    }

    curl_easy_cleanup(curl);
  }
  return response;
}

// GET 请求，带参数，返回传入字符+固定字符串
std::string getTest3(const std::string &param) {
  CURL *curl;
  CURLcode res;
  std::string response;
  std::string url = "http://47.115.44.145:7000/api/Test/test3?name=" + param;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    }

    curl_easy_cleanup(curl);
  }
  return response;
}

// POST 请求，带参数（Json 格式），返回 Json 字符串
std::string postTest4(const std::string &userName,
                      const std::string &passWord) {
  CURL *curl;
  CURLcode res;
  std::string response;

  curl = curl_easy_init();
  if (curl) {
    // 构造 JSON 字符串
    std::string jsonData =
        "{\"userName\":\"" + userName + "\",\"passWord\":\"" + passWord + "\"}";

    // 设置 URL 和请求头
    curl_easy_setopt(curl, CURLOPT_URL,
                     "http://47.115.44.145:7000/api/Test/test4");

    // 设置 Content-Type 为 application/json
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // 设置 POST 数据（JSON 字符串）
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

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

std::string postTest5(const std::string &userName,
                      const std::string &passWord) {
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

    // 设置回调函数和用户数据
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // 执行请求
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    }

    // 清理 mime 句柄
    curl_mime_free(form);
    // 清理 curl 句柄
    curl_easy_cleanup(curl);
  }
  return response;
}
// GET 请求，获取天气数据
std::string getWeatherForecast() {
  CURL *curl;
  CURLcode res;
  std::string response;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL,
                     "http://47.115.44.145:7000/WeatherForecast");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    }

    curl_easy_cleanup(curl);
  }
  return response;
}

int main() {
  std::cout << "Response from Test1: " << getTest1() << std::endl;
  std::cout << "Response from Test2: " << getTest2() << std::endl;
  std::cout << "Response from Test3: " << getTest3("123") << std::endl;

  std::string userName = "user1";
  std::string passWord = "password123";
  std::cout << "Response from Test4 (JSON): " << postTest4(userName, passWord)
            << std::endl;
  std::cout << "Response from Test5 (Form): " << postTest5(userName, passWord)
            << std::endl;

  std::cout << "Response from WeatherForecast: " << getWeatherForecast()
            << std::endl;

  return 0;
}

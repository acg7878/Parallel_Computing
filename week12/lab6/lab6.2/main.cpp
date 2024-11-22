#include <curl/curl.h>
#include <iostream>

// 回调函数，用于接收服务器返回的数据
size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                     std::string *output) {
  size_t totalSize = size * nmemb;
  output->append(static_cast<char *>(contents), totalSize);
  return totalSize;
}

void fetchURL(const std::string &url) {
  CURL *curl;
  CURLcode res;
  std::string responseString;

  // 初始化 CURL
  curl = curl_easy_init();
  if (curl) {
    // 设置 URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // 设置回调函数以处理返回数据
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    // 设置传递给回调函数的变量
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

    // 执行请求
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
    } else {
      std::cout << "Response from " << url << ":\n"
                << responseString << std::endl;
    }

    // 清理 CURL
    curl_easy_cleanup(curl);
  } else {
    std::cerr << "Failed to initialize CURL" << std::endl;
  }
}

int main() {
  // 接口 (1)
  std::string url1 = "http://47.115.44.145:7000/api/Test/test1";

  // 接口 (2)
  std::string url2 = "http://47.115.44.145:7000/api/Test/test2";

  std::cout << "Fetching data from Interface 1:" << std::endl;
  fetchURL(url1);

  std::cout << "\nFetching data from Interface 2:" << std::endl;
  fetchURL(url2);

  return 0;
}

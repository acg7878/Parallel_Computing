#include "http_client.hpp"
#include <curl/curl.h>
#include <iostream>


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *response) {
    size_t totalSize = size * nmemb;
    response->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

std::string postJson(const std::string &url, const std::string &jsonBody, const std::string &jwtToken) {
    CURL *curl = curl_easy_init();
    std::string response;

    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        if (!jwtToken.empty()) {
            headers = curl_slist_append(headers, ("Authorization: Bearer " + jwtToken).c_str());
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return response;
}
std::string postForm(const std::string &url, const std::string &userName, const std::string &passWord, const std::string &jwtToken) {
    CURL *curl = curl_easy_init();
    std::string response;

    if (curl) {
        // 初始化表单数据
        curl_mime *form = curl_mime_init(curl);
        curl_mimepart *field = nullptr;


        // 添加 userName 字段
        field = curl_mime_addpart(form);
        curl_mime_name(field, "userName");
        curl_mime_data(field, userName.c_str(), CURL_ZERO_TERMINATED);

        // 添加 passWord 字段
        field = curl_mime_addpart(form);
        curl_mime_name(field, "passWord");
        curl_mime_data(field, passWord.c_str(), CURL_ZERO_TERMINATED);

        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: multipart/form-data");

        if (!jwtToken.empty()) {
            headers = curl_slist_append(headers, ("Authorization: Bearer " + jwtToken).c_str());
        }
        // 设置请求参数
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // 执行请求
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // 释放资源
        curl_mime_free(form);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return response;
}


std::string httpGET(const std::string &url, const std::map<std::string, std::string> &headers ) {
    CURL *curl = curl_easy_init(); // 初始化 CURL
    std::string response;          // 保存响应内容

    if (!curl) {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return "";
    }

    // 设置 URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // 设置 HTTP 请求头
    struct curl_slist *curlHeaders = nullptr;
    for (const auto &header : headers) {
        std::string headerStr = header.first + ": " + header.second;
        curlHeaders = curl_slist_append(curlHeaders, headerStr.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curlHeaders);

    // 设置回调函数处理响应数据
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // 执行请求
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL GET request failed: " << curl_easy_strerror(res) << std::endl;
    }

    // 释放资源
    curl_easy_cleanup(curl);
    if (curlHeaders) {
        curl_slist_free_all(curlHeaders);
    }

    return response;
}
#include <curl/curl.h>
#include <iostream>
#include <string>
#include "../deps/include/sonic/sonic.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* responseData) ;
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* responseData) {
    size_t totalSize = size * nmemb;
    responseData->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// g++ -I../deps/include/ -march=haswell --std=c++11 test.cpp -o test_curl -lcurl
void test_curl() {
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        std::string method = "GET";
        curl_easy_setopt(curl, CURLOPT_URL, "www.baidu.com");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        std::string responseData;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        std::cout << responseData << std::endl;
    }

}

// g++ -I../deps/include/ -march=haswell --std=c++11 test.cpp -o test_sonic -lcurl
void test_sonic(){
  std::string json = R"(
    {
      "a": 1,
      "b": 2
    }
  )";

  sonic_json::Document doc;
  doc.Parse(json);

  sonic_json::WriteBuffer wb;
  doc.Serialize(wb);
  std::cout << wb.ToString() << std::endl;

}

// 示例 getFullKind 函数
std::string getFullKind(const std::string& shortKind, const std::string& apiVersion) {
    // 在这里实现您的 getFullKind 函数逻辑
    // ...
    // 这里只是一个示例，直接将 apiVersion 和 shortKind 拼接
    return apiVersion + "/" + shortKind;
}

void test_sonic2(){

    std::string resourceStringValues = R"(
        {
          "groupVersion": "v1",
          "resources": [
            {
              "kind": "Pod",
              "apiVersion": "v1"
            },
            {
              "kind": "Service",
              "apiVersion": "v1"
            }
          ]
        }
    )";

    // 解析 JSON 数据
    sonic_json::Document doc;
    doc.Parse(resourceStringValues);

    // 获取 groupVersion 值
    std::string apiVersion = doc["groupVersion"].GetString();

    std::cout << "----" << std::endl;
    std::cout << doc["resources"].GetString() << std::endl;  // 打印原始 JSON 数据
    std::cout << "----" << std::endl;
    /*
    for (const auto& w : doc["resources"].GetArray()) {
        const auto& resourceValue = w.GetObject();
        std::string shortKind = resourceValue["kind"].GetString();
        std::string fullKind = getFullKind(shortKind, apiVersion);
        std::cout << "Full Kind: " << fullKind << std::endl;
    }*/
    

}

int main() {
    // test_curl();
    // test_sonic();
    test_sonic2();
    return 0;
}


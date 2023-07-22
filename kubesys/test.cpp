#include <curl/curl.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
// #include "../deps/include/sonic/sonic.h"
using json = nlohmann::json;
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

/*
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
*/

// 示例 getFullKind 函数
std::string getFullKind(const std::string& shortKind, const std::string& apiVersion) {
    // 在这里实现您的 getFullKind 函数逻辑
    // ...
    // 这里只是一个示例，直接将 apiVersion 和 shortKind 拼接
    return apiVersion + "/" + shortKind;
}

void test_json(){
  // JSON字符串
    std::string jsonString = R"(
        {
            "name": "John",
            "age": 30,
            "is_student": true,
            "hobbies": ["reading", "swimming", "gaming"]
        }
    )";
    try {
        // 解析JSON数据
        json jsonData = json::parse(jsonString);

        // 获取JSON数据
        std::string name = jsonData["name"];
        int age = jsonData["age"];
        bool isStudent = jsonData["is_student"];

        // 获取数组数据
        // std::vector<std::string>
        auto hobbies = jsonData["hobbies"];

        // 输出JSON数据
        std::cout << "Name: " << name << std::endl;
        std::cout << "Age: " << age << std::endl;
        std::cout << "Is Student: " << std::boolalpha << isStudent << std::endl;

        std::cout << "Hobbies:" << std::endl;
        for (const auto& hobby : hobbies) {
            std::cout << "- " << hobby << std::endl;
        }

        // 将解析后的JSON数据再转换为字符串
        std::string parsedJsonString = jsonData.dump();
        std::cout << "Parsed JSON: " << parsedJsonString << std::endl;
    } catch (const std::exception& e) {
        // 解析失败
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
}
void test_common(){
  std::string word = "";
  std::cout << word.empty()<<std::endl;
}
int main() {
    // test_curl();
    // test_sonic();
    // test_sonic2();
    // test_json();
    test_common();
    return 0;
}


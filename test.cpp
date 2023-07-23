#include <curl/curl.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "./include/client.h"
#include "./include/util.h"
using json = nlohmann::json;
namespace kubesys {
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
  std::string word = "1";
  std::cout << word.empty()<<std::endl;
}

void test_kubesys(){
    KubernetesClient client("https://139.9.165.93:6443", "eyJhbGciOiJSUzI1NiIsImtpZCI6IkNhcVFxOHpmSHdRcTBpVFJvd2tacldzNzR2NElERHVzcG01eUM2ZmU0dHcifQ.eyJpc3MiOiJrdWJlcm5ldGVzL3NlcnZpY2VhY2NvdW50Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9uYW1lc3BhY2UiOiJrdWJlLXN5c3RlbSIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VjcmV0Lm5hbWUiOiJrdWJlcm5ldGVzLWNsaWVudC10b2tlbiIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VydmljZS1hY2NvdW50Lm5hbWUiOiJrdWJlcm5ldGVzLWNsaWVudCIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VydmljZS1hY2NvdW50LnVpZCI6IjYyMjc5ZWFiLTBiZmQtNGU2NC1hYjU3LTA3OGZiODhkMTk4MSIsInN1YiI6InN5c3RlbTpzZXJ2aWNlYWNjb3VudDprdWJlLXN5c3RlbTprdWJlcm5ldGVzLWNsaWVudCJ9.TI9mZC39ixMEq4h3cGfveKqLQSSh2y7BvvqLQixJBrFlKjsu9RJwlcGqEjh32UyshKDLtF4bM1J7w9HTMy7t74uGae9No-4Nm-R4kN3mXJA04MMdWZAV5gipDAuhR1J5R5wdoIIwYNyuUJNavWh61AqtXJkwC3uCAIYnClY9-Kx25Jif-XFlXyRkfETJxA2I9ZAbKZ3g_LOJgmVNfstjxSNTLJgRImYzQ65hrM2oZFul1_rZFPXM76rsNWwObvzPtDKPCT_yaqWt3dzAxxxOuP9EaQodVPSz7YNJb1ZHsGKgqAN9_I8MjQ2wJ0gLahyT4DFaU8rb2OvDhlDV66DoOw");
	
}
}
int main() {
    // g++  test.cpp -o test -lcurl
    using namespace kubesys;
    // test_curl();
    // test_sonic();
    // test_sonic2();
    // test_json();
    test_common();
    // test_kubesys();
    return 0;
}


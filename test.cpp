#include <curl/curl.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "./include/client.h"
#include "./include/util.h"
#include "./include/watcher.h"
#include "./deps/cppcodec-master/cppcodec/base64_rfc4648.hpp"
using json = nlohmann::json;
namespace kubesys {
// g++ -I../deps/include/ -march=haswell --std=c++11 test.cpp -o test_curl -lcurl
size_t WriteCallback2(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}
void test_curl() {
    // 初始化 curl
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "curl_easy_init failed." << std::endl;
    }

    // 设置要访问的 URL
    std::string url = "https://www.baidu.com";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // 设置 HTTP 头部，添加 Authorization 头部带上 Token
    // std::string token = "YourTokenHere";
    // struct curl_slist* headers = nullptr;
    // headers = curl_slist_append(headers, ("Authorization: Bearer " + token).c_str());
    // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // 设置接收 HTTP 响应的回调函数
    std::string response;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback2);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // 执行 HTTP 请求
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "HTTP request failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);

    }

    // 输出 HTTP 响应
    std::cout << "Response: " << response << std::endl;

    // 清理并释放资源
    curl_easy_cleanup(curl);
    // curl_slist_free_all(headers);
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
        for (const std::string &hobby: hobbies) {
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
  std::string word = "123";
  std::string url = "/apis/admissionregistration.k8s.io/v1";
  auto flag = url.find("/api") == 0;
  std::cout << word.empty()<<std::endl;
  std::cout <<  flag << std::endl;
}

std::string createPod() {
	return "{\n  \"apiVersion\": \"v1\",\n  \"kind\": \"Pod\",\n  \"metadata\": {\n    \"name\": \"busybox3\",\n    \"namespace\": \"default\"\n  },\n  \"spec\": {\n    \"containers\": [\n      {\n        \"image\": \"busybox\",\n        \"env\": [{\n           \"name\": \"abc\",\n           \"value\": \"abc\"\n        }],\n        \"command\": [\n          \"sleep\",\n          \"3600\"\n        ],\n        \"imagePullPolicy\": \"IfNotPresent\",\n        \"name\": \"busybox\"\n      }\n    ],\n    \"restartPolicy\": \"Always\"\n  }\n}";
}

void test_kubesys_token(){
    // KubernetesClient client("https://139.9.165.93:6443", "eyJhbGciOiJSUzI1NiIsImtpZCI6IkNhcVFxOHpmSHdRcTBpVFJvd2tacldzNzR2NElERHVzcG01eUM2ZmU0dHcifQ.eyJpc3MiOiJrdWJlcm5ldGVzL3NlcnZpY2VhY2NvdW50Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9uYW1lc3BhY2UiOiJrdWJlLXN5c3RlbSIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VjcmV0Lm5hbWUiOiJrdWJlcm5ldGVzLWNsaWVudC10b2tlbiIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VydmljZS1hY2NvdW50Lm5hbWUiOiJrdWJlcm5ldGVzLWNsaWVudCIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VydmljZS1hY2NvdW50LnVpZCI6IjYyMjc5ZWFiLTBiZmQtNGU2NC1hYjU3LTA3OGZiODhkMTk4MSIsInN1YiI6InN5c3RlbTpzZXJ2aWNlYWNjb3VudDprdWJlLXN5c3RlbTprdWJlcm5ldGVzLWNsaWVudCJ9.TI9mZC39ixMEq4h3cGfveKqLQSSh2y7BvvqLQixJBrFlKjsu9RJwlcGqEjh32UyshKDLtF4bM1J7w9HTMy7t74uGae9No-4Nm-R4kN3mXJA04MMdWZAV5gipDAuhR1J5R5wdoIIwYNyuUJNavWh61AqtXJkwC3uCAIYnClY9-Kx25Jif-XFlXyRkfETJxA2I9ZAbKZ3g_LOJgmVNfstjxSNTLJgRImYzQ65hrM2oZFul1_rZFPXM76rsNWwObvzPtDKPCT_yaqWt3dzAxxxOuP9EaQodVPSz7YNJb1ZHsGKgqAN9_I8MjQ2wJ0gLahyT4DFaU8rb2OvDhlDV66DoOw");
    // ubuntu token
    std::string response;
    std::string token = "eyJhbGciOiJSUzI1NiIsImtpZCI6IkF1V0FNbU5Dekluc2pWUUpkV3BpSTZZWktseG9PRTVyc0dHb25VdDdWdXcifQ.eyJpc3MiOiJrdWJlcm5ldGVzL3NlcnZpY2VhY2NvdW50Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9uYW1lc3BhY2UiOiJrdWJlLXN5c3RlbSIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VjcmV0Lm5hbWUiOiJrdWJlcm5ldGVzLWNsaWVudC10b2tlbiIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VydmljZS1hY2NvdW50Lm5hbWUiOiJrdWJlcm5ldGVzLWNsaWVudCIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VydmljZS1hY2NvdW50LnVpZCI6ImNkNzMyNThlLTJlY2MtNDNiYy05NTVlLTQ3YmI0NzUxMDhlMCIsInN1YiI6InN5c3RlbTpzZXJ2aWNlYWNjb3VudDprdWJlLXN5c3RlbTprdWJlcm5ldGVzLWNsaWVudCJ9.IV4yGnfB7bsgAOpdNCPErKbJpThpHGo41KwTIrRCoLwkUEJkXX2ngPbct0bXIx7TmyCy0c_U_Mpf956PeyWdh07AeK7dnnBQiRzSN5355-7CrJzmma3XOOWdteaI64gSbkcaSKvLdnYLWVA2yY4fEnaqc4giH4iaKme-4Xy1-GJthTYpJo3IyPAe40BN0LYewNLURPFce-nXVWKszP5KuVOP458_SK5qhh9MDcMCLYRHWspmcbjMeGK4RjH0cc08KSla-GUM5UdkS14rrBPoUDLuLb_VrsQYEwhXRhP7HE6HTwQd_ZJvrq4ABZphVcHKM2EAyqjUNSnlJNjcrKUMSQ"; 
    KubernetesClient client("https://192.168.203.130:6443",token);
    client.Init();
    // response = client.ListResources("Deployment","");
    // std::cout << "list ListResources: " <<response <<std::endl;
    
    response = client.CreateResource(createPod());
    std::cout << "create pod: " <<response <<std::endl;
    response = client.GetResource("Pod", "default", "busybox");
    
    std::cout << "get pod: "<< response <<std::endl;
}

void test_readfile() {
    auto path = "/etc/kubernetes/admin.conf";
    std::cout <<readFile(path) <<std::endl;
}
std::string base64_decode_string(const std::string& base64String) {
    std::vector<unsigned char> decodedData = cppcodec::base64_rfc4648::decode(base64String);
    std::string decodedString(decodedData.begin(), decodedData.end());
    return decodedString;
}


void test_kubesys_updateResource() {
    std::string response;
    KubernetesClient client("",CONFIGFILE);
    client.Init();
    response = client.GetResource("Pod", "default", "busybox");
    std::cout <<"GetResource name:" << nlohmann::json::parse(response)["metadata"]["name"] << std::endl;
    nlohmann::json obj = nlohmann::json::parse(response);
    if (obj.contains("metadata")) {
        auto& metadata = obj["metadata"];
        if (metadata.contains("annotations")) {
            metadata.erase("annotations");
        }
    }
    std::string updatedJson = obj.dump();

    std::cout << updatedJson << std::endl;
    response = client.UpdateResource(updatedJson);
    std::cout <<"UpdateResource response:" << response << std::endl;
}
void test_kubesys_b64() {
    std::string encode = "SGVsbG8sIHRoaXMgaXMgYSBzZWNyZXQgbWVzc2FnZSE=";
    auto encodedText = cppcodec::base64_rfc4648::decode(encode);
    std::string decodedString(encodedText.begin(), encodedText.end());
    std::cout << "decode Text: " <<  decodedString << std::endl;

    std::string response;
    KubernetesClient client("",CONFIGFILE);
    client.Init();
    response = client.ListResources("Deployment","");
    // std::cout << "ListResources: " <<response <<std::endl;


    response = client.CreateResource(createPod());
    std::cout << "create pod: " <<response <<std::endl;

    response = client.GetResource("Pod", "default", "busybox3");
    std::cout << "get pod: " <<nlohmann::json::parse(response)["metadata"]["name"] <<std::endl;

    // test_kubesys_updateResource();

    response = client.DeleteResource("Pod", "default", "busybox2");
    std::cout << "delete pod: " << response <<std::endl;

    auto url1 = "https://192.168.203.130:6443/api/v1/namespaces/default/pods/busybox1";
    auto url2 = "https://192.168.203.130:6443/api/v1/watch/namespaces/default/pods/busybox3/?watch=true&timeoutSeconds=20";
    auto url3 = "https://192.168.203.130:6443/api/v1/watch/namespaces/default/pods/busybox3/?watch=true&timeoutSeconds=1";
    DoHttpRequest(client.curl_,"GET",url3,"",response);
    std::cout << "watch pod: " << response <<std::endl;
    // std::shared_ptr<KubernetesClient> c(&client);
    // std::unique_ptr<WatchHandler> w(new PrintWatchHandler());
    // client.WatchResource("Pod", "default", "busybox",std::make_shared<KubernetesWatcher>(c,std::move(w)));
    }
}
int main() {
    // g++  test.cpp -o test -lcurl
    using namespace kubesys;
    // test_curl();
    // test_json();
    // test_common();
    // test_readfile();
    // test_kubesys_token();
    test_kubesys_b64();
    return 0;
}


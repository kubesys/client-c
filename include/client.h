#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "curl/curl.h"
#include "analyzer.h"
namespace kubesys {
    auto DoHttpRequest(CURL* curl_, const std::string& method, const std::string& url, const std::string& request,std::string& response) -> void ;
    class KubernetesClient {
    public:
        KubernetesClient(std::string &url, std::string &token);
        KubernetesClient(std::string& url, std::string& token, std::shared_ptr<KubernetesAnalyzer> analyzer);
        KubernetesClient(const std::string& kubeConfigFile = "/etc/kubernetes/admin.conf");
        ~KubernetesClient() {
            curl_easy_cleanup(curl_);
        };
        auto Init() -> void;
        auto BaseUrl(std::string &fullKind, std::string &nameSpace) -> std::string;
        auto BaseUrl2(std::string &fullKind, std::string &namespace) -> std::string;
        std::string url_;
        std::string token_;
    private:
        CURL *curl_ = curl_easy_init();
        std::shared_ptr<KubernetesAnalyzer> analyzer_;
    };
}
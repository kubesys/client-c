#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <curl/curl.h>
#include "analyzer.h"
#include "util.h"
namespace kubesys {
    typedef enum {
        TOKENFIlE,
        CONFIGFILE,
    } FileType;
    class KubernetesClient {
    public:
        KubernetesClient(const std::string &url, const std::string &token);
         //default "/var/run/secrets/kubernetes.io/serviceaccount/token"、"/etc/kubernetes/admin.conf"
         // when filepath is null, select default filepath
        KubernetesClient(std::string& filepath,FileType ft);    
        KubernetesClient(const std::string& url, const std::string& token, std::unique_ptr<KubernetesAnalyzer> analyzer);
        ~KubernetesClient() {
            curl_easy_cleanup(curl_);
        };
        auto Init() -> void;
        auto ListResources(const std::string& kind, const std::string& nameSpace) -> std::string;
        auto GetResource(const std::string& kind, const std::string& nameSpace, const std::string& name) -> std::string;
        auto CreateResource(const std::string &jsonStr) -> std::string;
        auto UpdateResource(const std::string &jsonStr) -> std::string;
        auto DeleteResource(const std::string& kind, const std::string& nameSpace, const std::string& name) -> std::string;
        std::string url_;
        std::string token_;
        CURL *curl_ = curl_easy_init();
    private:
        // extractor
        auto BaseUrl(const std::string &fullKind, const std::string &nameSpace) -> std::string;
        auto CreateResourceUrl(const std::string& fullKind, const std::string& nameSpace) -> std::string;
        auto ListResourcesUrl(const std::string& fullKind, const std::string& nameSpace) -> std::string;
        auto UpdateResourceUrl(const std::string& fullKind, const std::string& nameSpace, const std::string& name) -> std::string;
        auto DeleteResourceUrl(const std::string& fullKind, const std::string& nameSpace, const std::string& name) -> std::string;
        auto GetResourceUrl(const std::string& fullKind, const std::string& nameSpace, const std::string& name) -> std::string;
        auto UpdateResourceStatusUrl(const std::string& fullKind, const std::string& nameSpace, const std::string& name) -> std::string;
        auto BindingResourceStatusUrl(const std::string& fullKind, const std::string& nameSpace, const std::string& name) -> std::string;

        std::unique_ptr<KubernetesAnalyzer> analyzer_;
    };
}
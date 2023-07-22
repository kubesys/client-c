#include "../include/client.h"
#include "tls.cpp"
namespace kubesys {
   
    KubernetesClient::KubernetesClient(std::string& url, std::string& token)
        :url_(url), token_(token), analyzer_(std::make_unique<KubernetesAnalyzer>()) {
        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + token_).c_str());
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
    }

    KubernetesClient::KubernetesClient(std::string& url, std::string& token, std::shared_ptr<KubernetesAnalyzer> analyzer)
        :url_(url), token_(token), analyzer_(analyzer) {
        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + token_).c_str());
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
    }

    KubernetesClient::KubernetesClient(const std::string& kubeConfigFile = "/etc/kubernetes/admin.conf") {
        Config* config = NewForConfig(kubeConfigFile);
        // 设置客户端证书和密钥
        curl_easy_setopt(curl_, CURLOPT_SSLCERT, config->ClientCertificateData.c_str());
        curl_easy_setopt(curl_, CURLOPT_SSLKEY, config->ClientKeyData.c_str());
        // 设置服务器证书链（证书授权数据）
        curl_easy_setopt(curl_, CURLOPT_CAINFO, config->CertificateAuthorityData.c_str());

    }
    auto KubernetesClient::Init() -> void {
        if (analyzer_->ruleBase->KindToFullKindMapper.empty()) {

        }
    }

    size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
        size_t totalSize = size * nmemb;
        response->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    auto DoHttpRequest(CURL* curl_, const std::string& method, const std::string& url,  std::string& request,std::string& response) -> void {
        //exist requestbody,set content-type
        if (!request.empty()) {
            curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, body.c_str());
        }
        
        curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, method.c_str());

        std::string response;
        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response);
        
        auto res = curl_easy_perform(curl_);
        if (res != CURLE_OK) {
            std::cerr << "HTTP request failed: " << curl_easy_strerror(res) << std::endl;
        }
        
    }
    
}



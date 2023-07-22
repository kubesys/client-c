#include "../include/client.h"
#include "tls.cpp"
#include "util.cpp"
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

    KubernetesClient::KubernetesClient(const std::string&  tokenFile = "/var/run/secrets/kubernetes.io/serviceaccount/token") {
        // const std::string rootCAFile = "/var/run/secrets/kubernetes.io/serviceaccount/ca.crt";
        std::string host = std::getenv("KUBERNETES_SERVICE_HOST");
        std::string port = std::getenv("KUBERNETES_SERVICE_PORT");
        if (host.empty() || port.empty()) {
            throw std::runtime_error("unable to load in-cluster configuration, KUBERNETES_SERVICE_HOST and KUBERNETES_SERVICE_PORT must be defined");
        }
        std::string token = readFile(tokenFile);
        kubesys::checkedToken(token);
        url_ = "https://" + host + port ;
        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + token_).c_str());
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);

    }
    auto KubernetesClient::Init() -> void {
        if (analyzer_->ruleBase->KindToFullKindMapper.empty()) {

        }
    }

    auto KubernetesClient::BaseUrl(std::string &fullKind, std::string &nameSpace) -> std::string {
        std::string url = analyzer_->ruleBase->FullKindToApiPrefixMapper[fullKind] + "/";
        url += namespacePath(analyzer_->ruleBase->FullKindToNamespaceMapper[fullKind], nameSpace);
        url += analyzer_->ruleBase->FullKindToNameMapper[fullKind];
        return url;
    }

    size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
        size_t totalSize = size * nmemb;
        response->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    std::string namespacePath(bool supportNS, const std::string& ns) {
        if (supportNS && !ns.empty()) {
            // if a kind supports namespace, and namespace is not empty
            return "namespaces/" + ns + "/";
        }
        return "";
    }

    std::string fullKind(const std::string& jsonObj) {
        std::string kind, apiVersion;
        try {
            auto parsedJson = nlohmann::json::parse(jsonObj);
            kind = parsedJson["kind"].get<std::string>();
            apiVersion = parsedJson["apiVersion"].get<std::string>();
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            return ""; 
        }

        size_t index = apiVersion.find('/');
        if (index == std::string::npos) {
            return kind;
        }
        return apiVersion.substr(0, index) + "." + kind;
    }

    auto DoHttpRequest(CURL* curl_, const std::string& method, const std::string& url, const std::string& request,std::string& response) -> void {
        //exist requestbody,set content-type
        if (!request.empty()) {
            curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, request.c_str());
        }
        
        curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, method.c_str());

        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response);
        
        auto res = curl_easy_perform(curl_);
        if (res != CURLE_OK) {
            std::cerr << "HTTP request failed: " << curl_easy_strerror(res) << std::endl;
        }
    }
    
}



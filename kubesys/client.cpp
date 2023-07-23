#include "../include/client.h"
#include "../include/tls.h"

namespace kubesys {
   
    KubernetesClient::KubernetesClient(const std::string& url, const std::string& token)
        :url_(url), token_(token), analyzer_(std::make_unique<KubernetesAnalyzer>()) {
        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + token_).c_str());
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
    }

    KubernetesClient::KubernetesClient(const std::string& url, const std::string& token, std::unique_ptr<KubernetesAnalyzer> analyzer)
        :url_(url), token_(token), analyzer_(std::move(analyzer)) {
        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + token_).c_str());
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
    }

    KubernetesClient::KubernetesClient(std::string& filepath,FileType ft){
        if(ft == TOKENFIlE){
            if(filepath.empty()) {
                filepath = "/var/run/secrets/kubernetes.io/serviceaccount/token";
            }
            // const std::string rootCAFile = "/var/run/secrets/kubernetes.io/serviceaccount/ca.crt";
            std::string host = std::getenv("KUBERNETES_SERVICE_HOST");
            std::string port = std::getenv("KUBERNETES_SERVICE_PORT");
            if (host.empty() || port.empty()) {
                throw std::runtime_error("unable to load in-cluster configuration, KUBERNETES_SERVICE_HOST and KUBERNETES_SERVICE_PORT must be defined");
            }
            std::string token = readFile(filepath);
            kubesys::checkedToken(token);
            url_ = "https://" + host + port ;
            curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, ("Authorization: Bearer " + token_).c_str());
            curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
        } else if (ft == CONFIGFILE){
            if(filepath.empty()) {
                filepath = "/etc/kubernetes/admin.conf";
            }
            Config* config = NewForConfig(filepath);
            // 设置客户端证书和密钥
            curl_easy_setopt(curl_, CURLOPT_SSLCERT, config->ClientCertificateData.c_str());
            curl_easy_setopt(curl_, CURLOPT_SSLKEY, config->ClientKeyData.c_str());
            // 设置服务器证书链（证书授权数据）
            curl_easy_setopt(curl_, CURLOPT_CAINFO, config->CertificateAuthorityData.c_str());
            }
    }

    auto KubernetesClient::Init() -> void {
        if (analyzer_->ruleBase_->KindToFullKindMapper.empty()) {
            analyzer_->registry_->Learning(curl_,url_);
        }
    }

    auto KubernetesClient::ListResources(const std::string& kind, const std::string& nameSpace) -> std::string {
        std::string fullKind = toFullKind(kind, analyzer_->ruleBase_->KindToFullKindMapper);
        if(fullKind.empty()){
            return "";
        }
        std::string url = ListResourcesUrl(fullKind, nameSpace);
        std::string response;
        DoHttpRequest(curl_,"GET",url,"",response);
        return response;
    }

    
    
}



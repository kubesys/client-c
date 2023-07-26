/*
 Copyright (2023, )  Institute of Software, Chinese Academy of Sciences
 Author:    lvxin22@otcaix.iscas.ac.cn
            wuheng@iscas.ac.cn
*/

#include "../include/client.h"
#include "../include/tls.h"
#include "../include/watcher.h"

namespace kubesys {  
    KubernetesClient::KubernetesClient(const std::string& url, const std::string& token)
        :url_(url), token_(token), analyzer_(std::make_shared<KubernetesAnalyzer>()) {
        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + token_).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 0L);
    }

    KubernetesClient::KubernetesClient(const std::string& url, const std::string& token, std::shared_ptr<KubernetesAnalyzer> analyzer)
        :url_(url), token_(token), analyzer_(std::move(analyzer)) {
        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + token_).c_str());
        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 0L);
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
            curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 0L);
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
        std::cout << " ListResources url=" << url << std::endl;
        DoHttpRequest(curl_,"GET",url,"",response);
        return response;
    }

    auto KubernetesClient::GetResource(const std::string& kind, const std::string& nameSpace, const std::string& name) -> std::string {
        std::string fullKind = toFullKind(kind, analyzer_->ruleBase_->KindToFullKindMapper);
        if(fullKind.empty()){
            return "";
        }
        std::string url = GetResourceUrl(fullKind, nameSpace,name);
        std::string response;
        std::cout << " GetResource url=" << url << std::endl;
        DoHttpRequest(curl_,"GET",url,"",response);
        return nlohmann::json::parse(response)["metadata"]["name"];
    }

    auto KubernetesClient::CreateResource(const std::string &jsonStr) -> std::string {
        auto parsedJson = nlohmann::json::parse(jsonStr);
        std::string url = CreateResourceUrl(fullKind(parsedJson), getNamespace(parsedJson));
        std::string response;
        std::cout << " CreateResource url=" << url << std::endl;
        DoHttpRequest(curl_,"POST",url,jsonStr,response);
        return response;
    }

    auto KubernetesClient::UpdateResource(const std::string &jsonStr) -> std::string {
        auto parsedJson = nlohmann::json::parse(jsonStr);
        std::string url = UpdateResourceUrl(fullKind(parsedJson), getNamespace(parsedJson),getName(parsedJson));
        std::string response;
        std::cout << " UpdateResource url=" << url << std::endl;
        DoHttpRequest(curl_,"PUT",url,jsonStr,response);
        return response;
    }

    auto KubernetesClient::DeleteResource(const std::string& kind, const std::string& nameSpace, const std::string& name) -> std::string {
        std::string fullKind = toFullKind(kind, analyzer_->ruleBase_->KindToFullKindMapper);
        if(fullKind.empty()){
            return "";
        }
        std::string url = DeleteResourceUrl(fullKind, nameSpace,name);
        std::string response;
        std::cout << " DeleteResource url=" << url << std::endl;
        DoHttpRequest(curl_,"DELETE",url,"",response);
        return response;
    }

    auto KubernetesClient::UpdateResourceStatus(const std::string &jsonStr) -> std::string {
        auto parsedJson = nlohmann::json::parse(jsonStr);
        std::string url = UpdateResourceStatusUrl(fullKind(parsedJson), getNamespace(parsedJson), getName(parsedJson));
	    std::string response;
        std::cout << " UpdateResourceStatus url=" << url << std::endl;
        DoHttpRequest(curl_,"PUT",url,jsonStr,response);
        return response;
    }

    auto KubernetesClient::BindResources(nlohmann::json &pod , std::string &host) -> std::string {
        nlohmann::json podJson;
        podJson["apiVersion"] = "v1";
        podJson["kind"] = "Binding";

        nlohmann::json meta;
        meta["name"] = pod["metadata"]["name"].get<std::string>();
        meta["namespace"] = pod["metadata"]["namespace"].get<std::string>();
        podJson["metadata"] = meta;

        nlohmann::json target;
        target["apiVersion"] = "v1";
        target["kind"] = "Node";
        target["name"] = host;
        podJson["target"] = target;
        std::string url = BindingResourceStatusUrl(fullKind(pod), getNamespace(pod), getName(pod));
        std::string response;
        std::cout << " BindResources url=" << url << std::endl;
        DoHttpRequest(curl_,"POST",url,podJson.dump(),response);
        return response;
    }
    
    auto KubernetesClient::WatchResource(const std::string &kind , const std::string &nameSpace, const std::string &name , std::shared_ptr<KubernetesWatcher> watcher) -> void {
        std::string fullKind = toFullKind(kind,analyzer_->ruleBase_->KindToFullKindMapper);
        if (fullKind.empty()) {
            return ;
        }
        std::string url = analyzer_->ruleBase_->FullKindToApiPrefixMapper[fullKind] + "/watch/";
        url += namespacePath(analyzer_->ruleBase_->FullKindToNamespaceMapper[fullKind], nameSpace);
        url += analyzer_->ruleBase_->FullKindToNameMapper[fullKind] + "/" + name;
        url += "/?watch=true&timeoutSeconds=315360000";
        watcher->Watching(url);
    }

    auto KubernetesClient::WatchResources(const std::string &kind , const std::string &nameSpace, std::shared_ptr<KubernetesWatcher> watcher) -> void {
        std::string fullKind = toFullKind(kind,analyzer_->ruleBase_->KindToFullKindMapper);
        if (fullKind.empty()) {
            return ;
        }
        std::string url = analyzer_->ruleBase_->FullKindToApiPrefixMapper[fullKind] + "/watch/";
        url += namespacePath(analyzer_->ruleBase_->FullKindToNamespaceMapper[fullKind], nameSpace);
        url += analyzer_->ruleBase_->FullKindToNameMapper[fullKind];
        url += "/?watch=true&timeoutSeconds=315360000";
        watcher->Watching(url);
    }
    
    auto KubernetesClient::ListResourcesWithLabelSelector(const std::string &kind , const std::string &nameSpace, std::map<std::string,std::string> labels) ->std::string {
        auto fullKind = toFullKind(kind, analyzer_->ruleBase_->KindToFullKindMapper);
        if(fullKind.empty()) {
            return "";
        }
        auto url = ListResourcesUrl(fullKind, nameSpace) + "?labelSelector=";
        for (const auto& label : labels) {
            url += label.first + "%3D" + label.second + ",";
        }
        url = url.substr(0, url.size() - 1);
        std::string response;
        std::cout << " ListResourcesWithLabelSelector url=" << url << std::endl;
        DoHttpRequest(curl_,"GET",url,"",response);
        return response;
    }

    auto KubernetesClient::ListResourcesWithFieldSelector(const std::string &kind , const std::string &nameSpace, std::map<std::string,std::string> fields) -> std::string {
        auto fullKind = toFullKind(kind, analyzer_->ruleBase_->KindToFullKindMapper);
        if(fullKind.empty()) {
            return "";
        }
        auto url = ListResourcesUrl(fullKind, nameSpace) + "?fieldSelector=";
        for (const auto& f : fields) {
            url += f.first + "%3D" + f.second + ",";
        }
        url = url.substr(0, url.size() - 1);
        std::string response;
        std::cout << " ListResourcesWithFieldSelector url=" << url << std::endl;
        DoHttpRequest(curl_,"GET",url,"",response);
        return response;
    }
   
    auto KubernetesClient::GetKinds() -> std::vector<std::string> {
        std::vector<std::string> keys;
        for(const auto& pair: analyzer_->ruleBase_->KindToFullKindMapper) {
            keys.push_back(pair.first);
        }
        return keys;
    }

    auto KubernetesClient::GetFullKinds() -> std::vector<std::string> {
        std::vector<std::string> keys;
        for(const auto& pair: analyzer_->ruleBase_->FullKindToNameMapper) {
            keys.push_back(pair.first);
        }
        return keys;
    }

    auto KubernetesClient::GetKindDesc() -> std::string {
        nlohmann::json desc;
        for (const auto& pair : analyzer_->ruleBase_->FullKindToNameMapper) {
                const std::string& fullKind = pair.first;
                nlohmann::json value;
                value["apiVersion"] = analyzer_->ruleBase_->FullKindToVersionMapper[fullKind];
                value["kind"] = kind(fullKind);
                value["plural"] = pair.second;
                value["verbs"] = analyzer_->ruleBase_->FullKindToVerbsMapper[fullKind];
                desc[fullKind] = value;
        }
        return desc.dump();
    }
}

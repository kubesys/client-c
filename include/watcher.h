#pragma once
#include <iostream>
#include <map>
#include <memory>
#include "client.h"
namespace kubesys {
class WatchHandler {
public:
    virtual void DoAdded(std::map<std::string, std::string> obj) = 0;
    virtual void DoModified(std::map<std::string, std::string> obj) = 0;
    virtual void DoDeleted(std::map<std::string, std::string> obj) = 0;
    virtual ~WatchHandler() {}
};
class KubernetesWatcher {
private:
    std::unique_ptr<KubernetesClient> client_;
    std::unique_ptr<WatchHandler> handler_;

public:
    KubernetesWatcher(std::unique_ptr<KubernetesClient> client, std::unique_ptr<WatchHandler> handler) : client_(std::move(client)), handler_(std::move(handler)) {};
    void Watching(std::string url) {
        auto wclinet = KubernetesClient(url, client_->token_, client_->analyzer_);
        wclinet.curl_ = client_->curl_;
        std::string response;
        DoHttpRequest(wclinet.curl_, "GET", url, "", response);
        std::istringstream iss(response);
        std::string line;
        while (std::getline(iss, line)) {
            std::cout << line << std::endl; 
            nlohmann::json jsonData = nlohmann::json::parse(response);
            if(jsonData["type"] == "ADDED") {
                handler_->DoAdded(jsonData["object"]);
            }else if(jsonData["type"] == "MODIFIED"){
                handler_->DoModified(jsonData["object"]);
            }else if(jsonData["type"] == "DELETED") {
                handler_->DoDeleted(jsonData["object"]);
            }
        }
    }
};

// 示例实现 WatchHandler 的具体类
class PrintWatchHandler  : public WatchHandler {
public:
    void DoAdded(std::map<std::string, std::string> obj) override {
        nlohmann::json jsonObject(obj);
        std::cout << "ADDED: " << jsonObject.dump() << std::endl;
    }

    void DoModified(std::map<std::string, std::string> obj) override {
        nlohmann::json jsonObject(obj);
        std::cout << "MODIFIED: " << jsonObject.dump() << std::endl;
    }

    void DoDeleted(std::map<std::string, std::string> obj) override {
        nlohmann::json jsonObject(obj);
        std::cout << "DELETED: " << jsonObject.dump() << std::endl;
    }
};
}


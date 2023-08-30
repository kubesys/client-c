#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "client.h"
namespace kubesys {
class WatchHandler {
public:
    virtual void DoAdded(nlohmann::json jsonData) = 0;
    virtual void DoModified(nlohmann::json jsonData) = 0;
    virtual void DoDeleted(nlohmann::json jsonData) = 0;
    virtual ~WatchHandler() {}
};
class KubernetesWatcher {
private:
    std::shared_ptr<KubernetesClient> client_;
    std::unique_ptr<WatchHandler> handler_;

public:
    KubernetesWatcher(std::shared_ptr<KubernetesClient> client, std::unique_ptr<WatchHandler> handler) : client_(client), handler_(std::move(handler)) {};
    void Watching(std::string url) {
        // auto newcurl = curl_easy_duphandle(client_->curl_);
        // auto wclinet = KubernetesClient(url, client_->analyzer_, newcurl);
        std::string response;
        std::cout<< "watch url:" << url <<std::endl;
        DoHttpRequest(client_->curl_, "GET", url, "", response);
        std::istringstream iss(response);
        std::string line;
        while (std::getline(iss, line)) {
            nlohmann::json jsonData = nlohmann::json::parse(line);
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

class PrintWatchHandler  : public WatchHandler {
public:
    void DoAdded(nlohmann::json jsonData) override {
        std::cout << "ADDED:" <<  jsonData.dump() << std::endl;
        auto phase = jsonData["status"]["phase"].get<std::string>();
        auto name = jsonData["metadata"]["name"].get<std::string>();
        if(phase == "Pending"){
            
        }
    }

    void DoModified(nlohmann::json jsonData) override {
        std::cout << "MODIFIED:" <<  jsonData.dump() << std::endl;
    }

    void DoDeleted(nlohmann::json jsonData) override {
        std::cout << "DELETED:" <<  jsonData.dump() << std::endl;
    }
};
}


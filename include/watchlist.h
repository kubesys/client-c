#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "client.h"
namespace kubesys {
class WatchListHandler {
public:
    virtual void DoAdded(nlohmann::json    &watchJson,   nlohmann::json  &listJson) = 0;
    virtual void DoModified(nlohmann::json &watchJson,   nlohmann::json  &list) = 0;
    virtual void DoDeleted(nlohmann::json  &watchJson,   nlohmann::json  &list) = 0 ;
    virtual ~WatchListHandler(){};
};

class KubernetesWatchLister {
public:
    std::shared_ptr<KubernetesClient> client_;
    std::unique_ptr<WatchListHandler> handler_;
    KubernetesWatchLister(std::shared_ptr<KubernetesClient> client, std::unique_ptr<WatchListHandler> handler) : client_(client), handler_(std::move(handler)) {};
    void WatchList(std::string url) {
        // auto newcurl = curl_easy_duphandle(client_->curl_);
        // auto wclinet = KubernetesClient(url, client_->analyzer_, newcurl);
        std::string watch_pod,list_node;
        std::cout<< "watch url:" << url <<std::endl;
        DoHttpRequest(client_->curl_, "GET", url, "", watch_pod);
        list_node = client_->ListResources("Node","");
        auto listJson = nlohmann::json::parse(list_node);
        
        nlohmann::json nodesArray;
        for (const auto& item : listJson["items"]) {
            nlohmann::json extractedItem;
            std::string name = item["meta"]["name"];
            nlohmann::json status = item["status"];

            extractedItem["name"] = name;
            extractedItem["status"] = status;
            nodesArray.push_back(extractedItem);
        }

        std::istringstream iss(watch_pod);
        std::string line;
        while (std::getline(iss, line)) {
            nlohmann::json jsonData = nlohmann::json::parse(line);
            if(jsonData["type"] == "ADDED") {
                handler_->DoAdded(jsonData,nodesArray);
            }else if(jsonData["type"] == "MODIFIED"){
                handler_->DoModified(jsonData["object"],nodesArray);
            }else if(jsonData["type"] == "DELETED") {
                handler_->DoDeleted(jsonData["object"],nodesArray);
            }
        }
    }
};
}


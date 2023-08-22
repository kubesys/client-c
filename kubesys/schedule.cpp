#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "../include/watcher.h"
namespace kubesys {

class ScheduleWatchHandler  : public WatchHandler {
public:
    void DoAdded(nlohmann::json jsonData) override {
        std::cout << "ADDED:" <<  jsonData.dump() << std::endl;
        auto phase = jsonData["status"]["phase"].get<std::string>();
        auto name = jsonData["metadata"]["name"].get<std::string>();
        auto urls = "";
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


#include "../../include/schedule/schedule.h"
#include "../../include/watcher.h"

namespace kubesys {

class fwImpl  : public frameworker {
public:
    void filter() override{};
    void score() override{};
};

void Informer::Run(){
    std::unique_ptr<WatchHandler> w(new PrintWatchHandler());
    watchlist_->client_->WatchResources("Pod", "default",std::make_shared<KubernetesWatcher>(client,std::move(w)))
}

void Schedule::ScheduleOne() {
    findNodesFitpod();
    prioritizeNodes();
    selectHost();
};

class PrintWatchListHandler  : public WatchListHandler {
public:
    void DoAdded(nlohmann::json &watchJson, nlohmann::json  &listJson) override {
        std::cout << "ADDED:" <<  watchJson.dump() << std::endl;
        auto phase = watchJson["status"]["phase"].get<std::string>();
        auto name = watchJson["metadata"]["name"].get<std::string>();
        if(phase == "Pending"){
            
        }
    }

    void DoModified(nlohmann::json &watchJson, nlohmann::json  &listJson) override {
        std::cout << "MODIFIED:" <<  watchJson.dump() << std::endl;
    }

    void DoDeleted(nlohmann::json &watchJson, nlohmann::json  &listJson) override {
        std::cout << "DELETED:" <<  watchJson.dump() << std::endl;
    }
};
}
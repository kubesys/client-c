#include "../../include/schedule/schedule.h"
#include "../../include/watcher.h"

namespace kubesys {

class fwImpl  : public frameworker {
public:
    fwImpl(){};
    void filter() {};
    std::map<std::string,float> score(nlohmann::json nodes) {
        std::map<std::string,float> result;
        for(auto &node :nodes) {
            // node_status["allocatable"]["memory"]) / float(node_status["capacity"]["memory"]
            std::string alloc_memory = nodes["allocatable"]["memory"];
            std::string total_memory = nodes["capacity"]["memory"];
            std::string name = node["name"];
            result[name] = std::stof(alloc_memory.substr(0,alloc_memory.size()-2))/std::stof(total_memory.substr(0,alloc_memory.size()-2));
        }
        return result;
    }
};

void Informer::Run(){
    watchlist_->client_->WatchListResources("Pod", "default",watchlist_);
}

void Schedule::ScheduleOne(nlohmann::json pod) {
    findNodesFitpod(store_->nodesInfo);
    auto result = prioritizeNodes(store_->nodesInfo);
    selectHost(result);
};

void Schedule::Run(){
    std::thread myThread([this]() {
        informer_->Run();
    });
    myThread.detach();

    int size = 0;
    nlohmann::json pod;
    while(true) {
        std::unique_lock<std::mutex> lock(store_->queueMutex);
        store_->enqueCond_.wait(lock, [this,&pod] {
            if(store_->podsQueue_.empty()) {
                return false;
            }
            pod = store_->podsQueue_.front();
            store_->podsQueue_.pop();
            return true;
        });
        ScheduleOne(pod);
    }
}

void Schedule::findNodesFitpod(nlohmann::json nodes) {

}

std::map<std::string,float> Schedule::prioritizeNodes(nlohmann::json nodes){
    return f_->score(nodes);
}

std::string Schedule::selectHost(std::map<std::string,float> result){
    std::string name;
    float score = 0;
    for(auto &[k,v]:result) {
        if(v > score) {
            score = v;
            name = k;
        }
    }
    std::cout << "select " << name << std::endl;
    return name;
}

class WatchListHandlerImpl  : public WatchListHandler {
public:
    
    WatchListHandlerImpl(store *s):store_(s){};

    void DoAdded(nlohmann::json &watchJson, nlohmann::json  &listJson) override {
        std::cout << "ADDED:" <<  watchJson.dump() << std::endl;
        auto phase = watchJson["status"]["phase"].get<std::string>();
        auto name = watchJson["metadata"]["name"].get<std::string>();
        if(phase == "Pending"){
            std::unique_lock<std::mutex> lock(store_->queueMutex);
            store_->podsQueue_.push(watchJson);
            store_->nodesInfo = listJson;
            if(store_->podsQueue_.empty()) {
                store_->enqueCond_.notify_one();
            }
        }
    }

    void DoModified(nlohmann::json &watchJson, nlohmann::json  &listJson) override {
        std::cout << "MODIFIED:" <<  watchJson.dump() << std::endl;
    }

    void DoDeleted(nlohmann::json &watchJson, nlohmann::json  &listJson) override {
        std::cout << "DELETED:" <<  watchJson.dump() << std::endl;
    }
private:
    std::shared_ptr<store> store_;
};
}
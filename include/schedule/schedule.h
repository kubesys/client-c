#pragma once
#include "../watchlist.h"
#include <memory>
#include <queue>
#include <thread>
namespace kubesys {

typedef struct store {
    std::condition_variable enqueCond_; 
    std::mutex queueMutex;
    std::queue<nlohmann::json> podsQueue_;
    nlohmann::json nodesInfo;
}store;
class frameworker {
public:
    virtual void filter() = 0 ;
    virtual std::map<std::string,float> score(nlohmann::json nodes) = 0;
    ~frameworker(){};
};

class Informer {
public:
    
    std::shared_ptr<struct store> store_;
    std::shared_ptr<KubernetesWatchLister>watchlist_;
    Informer(){};
    Informer(std::shared_ptr<KubernetesClient> client, std::unique_ptr<WatchListHandler> handler, store *s)
            : watchlist_(std::make_shared<KubernetesWatchLister>(client,std::move(handler))),store_(s) {};
    void Run();
    ~Informer() {};

    
};

class Schedule {
public:
    frameworker *f_;
    std::shared_ptr<Informer>informer_;
    Schedule(frameworker *f,std::shared_ptr<Informer> informer):f_(f),informer_(informer){};
    ~Schedule(){};
    void Run();
    void ScheduleOne(nlohmann::json nodes);
private:
    std::shared_ptr<struct store> store_;
    void findNodesFitpod(nlohmann::json nodes);
    std::map<std::string,float> prioritizeNodes(nlohmann::json nodes);
    std::string selectHost(std::map<std::string,float> result);
};
}
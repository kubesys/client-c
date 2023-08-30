#include "../watchlist.h"

namespace kubesys {
class frameworker {
public:
    virtual void filter() ;
    virtual void score() ;
    virtual ~frameworker() {}
};

class Informer {
public:
    std::condition_variable enqueCond_; 
    std::string nodes_;
    KubernetesWatchLister watchlist_;
    Informer(std::shared_ptr<KubernetesClient> client, std::unique_ptr<WatchListHandler> handler)
            : watchlist_(KubernetesWatchLister(client,std::move(handler))) {};
    void Run();
    ~Informer() {}
};

class Schedule {
public:
    frameworker f;
    Schedule();
    ~Schedule();
    void ScheduleOne();
private:
    void findNodesFitpod();
    void prioritizeNodes();
    void selectHost();
}
}
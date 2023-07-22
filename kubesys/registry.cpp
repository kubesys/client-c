#include "../include/registry.h"
#include "../deps/include/sonic/sonic.h"
namespace kubesys{
   
        Registry::Registry(std::shared_ptr<RuleBase>ruleBase): ruleBase(ruleBase) {};
        auto Registry::Learning(CURL *curl,std::string &url) -> void {
        std::string response;
            DoHttpRequest(curl, "GET", url, "",response);
            sonic_json::Document doc;
            doc.Parse(doc);
        }
}
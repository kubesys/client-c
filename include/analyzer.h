#pragma once
#include "rulebase.h"
#include "registry.h"
#include "curl/curl.h"
#include "client.h"
namespace kubesys {
    class KubernetesAnalyzer {
    public:
        KubernetesAnalyzer(std::shared_ptr<RuleBase> ruleBase, std::unique_ptr<Registry> registry);
        auto Learning(CURL *curl,std::string &url) -> void;
        // Other member functions and logic for KubernetesAnalyzer
        std::shared_ptr<RuleBase> ruleBase;
        std::unique_ptr<Registry> registry;
    };
}
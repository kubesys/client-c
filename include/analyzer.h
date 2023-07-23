#pragma once
#include "rulebase.h"
#include "registry.h"
namespace kubesys {
    class KubernetesAnalyzer {
    public:
        KubernetesAnalyzer();
        KubernetesAnalyzer(std::shared_ptr<RuleBase> ruleBase, std::unique_ptr<Registry> registry);
        // Other member functions and logic for KubernetesAnalyzer
        std::shared_ptr<RuleBase> ruleBase_;
        std::unique_ptr<Registry> registry_;
    };
}
#include <memory> 
#include "../include/analyzer.h"

namespace kubesys {

    KubernetesAnalyzer::KubernetesAnalyzer(std::shared_ptr<RuleBase> ruleBase, std::unique_ptr<Registry> registry)
        : ruleBase(std::move(ruleBase)), registry(std::move(registry)) {}

    
}

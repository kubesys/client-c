/*
 Copyright (2023, )  Institute of Software, Chinese Academy of Sciences
 Author:    lvxin22@otcaix.iscas.ac.cn
            wuheng@iscas.ac.cn
*/

#include <memory> 
#include "../include/analyzer.h"

namespace kubesys {
    KubernetesAnalyzer::KubernetesAnalyzer(){
        ruleBase_ = std::make_shared<RuleBase>();
        registry_ = std::make_unique<Registry>(Registry(ruleBase_));
    }
    KubernetesAnalyzer::KubernetesAnalyzer(std::shared_ptr<RuleBase> ruleBase, std::unique_ptr<Registry> registry)
        : ruleBase_(std::move(ruleBase)), registry_(std::move(registry)) {}

    
}

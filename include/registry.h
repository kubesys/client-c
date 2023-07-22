#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "client.h"
#include "rulebase.h"
namespace kubesys{
    class Registry {
    public:
        Registry(std::shared_ptr<RuleBase>ruleBase): ruleBase(ruleBase) {}
        ~Registry() {
            // ruleBase.reset(); // Release RuleBase object
        }
        auto Learning(CURL *curl,std::string &url) -> void;
        std::shared_ptr<RuleBase> ruleBase;
    };
}
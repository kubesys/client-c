#pragma once
#include <iostream>
#include <string_view>
#include <memory>
#include "rulebase.h"
#include <curl/curl.h>
namespace kubesys{
    class Registry {
    public:
        Registry(std::shared_ptr<RuleBase>ruleBase): ruleBase_(ruleBase) {}
        ~Registry() {
            // ruleBase.reset(); // Release RuleBase object
        }
        auto Learning(CURL *curl, std::string &url) -> void;
        auto Register(CURL *curl, std::string url) -> void;
        std::shared_ptr<RuleBase> ruleBase_;
    };
}
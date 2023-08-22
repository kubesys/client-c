#pragma once
#include <nlohmann/json.hpp>

namespace kubesys {
class PodSchedule {
    virtual std::string schedule(const nlohmann::json &pod,const nlohmann::json &nodes) = 0;
};
}
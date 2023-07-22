#pragma once
#include <string>
#include <map>
#include <vector>

namespace kubesys{
    struct RuleBase {
        std::map<std::string, std::vector<std::string>> KindToFullKindMapper;
        std::map<std::string, std::string> FullKindToApiPrefixMapper;
        std::map<std::string, std::string> FullKindToNameMapper;
        std::map<std::string, bool> FullKindToNamespaceMapper;
        std::map<std::string, std::string> FullKindToVersionMapper;
        std::map<std::string, std::string> FullKindToGroupMapper;
        std::map<std::string, std::vector<std::string>> FullKindToVerbsMapper;
    };
}
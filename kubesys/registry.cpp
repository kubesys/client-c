/*
 Copyright (2023, )  Institute of Software, Chinese Academy of Sciences
 Author:    lvxin22@otcaix.iscas.ac.cn
            wuheng@iscas.ac.cn
*/

#include "../include/registry.h"
#include <nlohmann/json.hpp>
#include "../include/util.h"

namespace kubesys{
    // 比较两个字符串是否相等，忽略大小写
    auto isEqualIgnoreCase(const std::string& str1, const std::string& str2) -> bool {
        return str1.size() == str2.size() &&
            std::equal(str1.begin(), str1.end(), str2.begin(), [](char c1, char c2) {
                return std::tolower(c1) == std::tolower(c2);
            });
    }

    auto getFullKind(const std::string& shortKind, const std::string& apiVersion) -> std::string {
        size_t index = apiVersion.find('/');
        std::string apiGroup = "";
        if (index != std::string::npos) {
            apiGroup = apiVersion.substr(0, index);
        }

        std::string fullKind = "";
        if (apiGroup.empty()) {
            fullKind = shortKind;
        } else {
            fullKind = apiGroup + "." + shortKind;
        }
        return fullKind;
    }

    auto getGroup(const std::string& apiVersion) -> std::string {
        size_t index = apiVersion.find_last_of('/');
        if (index != std::string::npos && index > 0) {
            return apiVersion.substr(0, index);
        }
        return "";
    }
    

    auto Registry::Learning(CURL *curl, std::string &url) -> void {
        std::string response;
        DoHttpRequest(curl, "GET", url, "",response);
        nlohmann::json jsonData = nlohmann::json::parse(response);
        std::cout << "Learning url:" << url  << std::endl;
        for(std::string path: jsonData["paths"]) {
            if (path.find("/api") == 0 && (std::count(path.begin(), path.end(), '/') == 3  || isEqualIgnoreCase(path,"/api/v1"))){
                Register(curl,url+path);
            }
        }
    }
    auto Registry::Register(CURL *curl, std::string url) -> void {
        std::string response;
        DoHttpRequest(curl, "GET", url, "",response);
        nlohmann::json jValues = nlohmann::json::parse(response);
        std::string apiVersion = jValues["groupVersion"];
        for (const auto& w : jValues["resources"]) {
            nlohmann::json jValue = w;
            std::string shortKind = jValue["kind"].get<std::string>();
            std::string fullKind = getFullKind(shortKind, apiVersion);
            if(ruleBase_->FullKindToApiPrefixMapper.count(fullKind) == 0) {
                ruleBase_->KindToFullKindMapper[shortKind].push_back(fullKind);
                ruleBase_->FullKindToApiPrefixMapper[fullKind] = url; 
                ruleBase_->FullKindToNameMapper[fullKind] = jValue["name"].get<std::string>();
                ruleBase_->FullKindToNamespaceMapper[fullKind] = jValue["namespaced"].get<bool>();
                ruleBase_->FullKindToVersionMapper[fullKind] = apiVersion;
                ruleBase_->FullKindToGroupMapper[fullKind] = getGroup(apiVersion);
                ruleBase_->FullKindToVerbsMapper[fullKind] = jValue["verbs"].get<std::vector<std::string>>();
            }
        }
    }
}
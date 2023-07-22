#include "../include/registry.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace kubesys{
        Registry::Registry(std::shared_ptr<RuleBase>ruleBase): ruleBase_(ruleBase) {};

        // 比较两个字符串是否相等，忽略大小写
        auto isEqualIgnoreCase(const std::string& str1, const std::string& str2) -> bool {
            return str1.size() == str2.size() &&
                std::equal(str1.begin(), str1.end(), str2.begin(), [](char c1, char c2) {
                    return std::tolower(c1) == std::tolower(c2);
                });
        }

        auto getFullKind(const std::string& apiVersion, const std::string& shortKind) -> std::string {
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
        

        auto Registry::Learning(CURL *curl,std::string &url) -> void {
            std::string response;
            DoHttpRequest(curl, "GET", url, "",response);
            json jsonData = json::parse(response);
            for(std::string path: jsonData['path']) {
                if (url.find("/api") == 0 && (std::count(path.begin(), path.end(), '/') == 3 ) || isEqualIgnoreCase(path,"/api/v1")){

                }
            }
        }

        /*
    apiVersion := resourceValues["groupVersion"].(string)

	for _, w := range resourceValues["resources"].([]interface{}) {
		resourceValue := w.(map[string]interface{})
		shortKind := resourceValue["kind"].(string)
		fullKind := getFullKind(resourceValue, shortKind, apiVersion)

		if _, ok := registry.RuleBase.FullKindToApiPrefixMapper[fullKind]; !ok {
			registry.RuleBase.KindToFullKindMapper[shortKind] = append(registry.RuleBase.KindToFullKindMapper[shortKind], fullKind)
			registry.RuleBase.FullKindToApiPrefixMapper[fullKind] = url

			registry.RuleBase.FullKindToNameMapper[fullKind] = resourceValue["name"].(string)
			registry.RuleBase.FullKindToNamespaceMapper[fullKind] = resourceValue["namespaced"].(bool)

			registry.RuleBase.FullKindToVersionMapper[fullKind] = apiVersion
			registry.RuleBase.FullKindToGroupMapper[fullKind] = getGroup(apiVersion)
			registry.RuleBase.FullKindToVerbsMapper[fullKind] = resourceValue["verbs"]
		}
	}
        */
        auto Registry::Register(CURL *curl,std::string &url) -> void {
            std::string response;
            DoHttpRequest(curl, "GET", url, "",response);
            json jValues = json::parse(response);
            std::string apiVersion = jValues["groupVersion"];
            for (const auto& w : jValues["resources"]) {
                nlohmann::json jValue = w;
                std::string shortKind = jValue["kind"].get<std::string>();
                std::string fullKind = getFullKind(shortKind, apiVersion);
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
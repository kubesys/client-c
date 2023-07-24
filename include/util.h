#pragma once
#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <optional>
#include <nlohmann/json.hpp>
#include <fstream>
#include <curl/curl.h>
namespace kubesys{
    size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response);
    bool DoHttpRequest(CURL* curl_, const std::string& method, const std::string& url, const std::string& request,std::string& response);
    std::string checkedUrl(const std::string& url);
    std::string checkedToken(const std::string& token);
    nlohmann::json ToJsonObject(const std::vector<uint8_t>& bytes);
    std::map<std::string, nlohmann::json> ToGolangMap(const std::vector<uint8_t>& bytes);
    std::string readFile(const std::string& filePath);
    std::string toFullKind(const std::string& kind, std::map<std::string, std::vector<std::string>>& mapper);
    std::string namespacePath(bool supportNS, const std::string& ns);
    std::string fullKind(const nlohmann::json& parsedJson);
    std::string getNamespace(const nlohmann::json& jsonObj);
    std::string getName(const nlohmann::json& jsonObj);
}
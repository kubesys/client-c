#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;
namespace kubesys {
    std::string checkedUrl(const std::string& url) {
        std::regex httpsRegExp("https:\\/\\/([\\w.]+\\/?)\\S*");
        if (!std::regex_match(url, httpsRegExp)) {
            throw std::runtime_error("just support https without suffix '/'");
        }
        return url;
    }

    std::string checkedToken(const std::string& token) {
        if (token.empty()) {
            throw std::runtime_error("token is empty");
        }
        return token;
    }

    json ToJsonObject(const std::vector<uint8_t>& bytes) {
        std::string jsonStr(bytes.begin(), bytes.end());
        return json::parse(jsonStr);
    }

    std::map<std::string, json> ToGolangMap(const std::vector<uint8_t>& bytes) {
        std::string jsonStr(bytes.begin(), bytes.end());
        return json::parse(jsonStr).get<std::map<std::string, json>>();
    }

    std::string readFile(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filePath);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}

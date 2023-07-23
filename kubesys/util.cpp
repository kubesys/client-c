#include "../include/util.h"
using json = nlohmann::json;
namespace kubesys {
    size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
        size_t totalSize = size * nmemb;
        response->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    auto DoHttpRequest(CURL* curl_, const std::string& method, const std::string& url, const std::string& request,std::string& response) -> void {
        //exist requestbody,set content-type
        if (!request.empty()) {
            curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, request.c_str());
        }
        
        curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, method.c_str());

        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response);
        
        auto res = curl_easy_perform(curl_);
        if (res != CURLE_OK) {
            std::cerr << "HTTP request failed: " << curl_easy_strerror(res) << std::endl;
        }
    }

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

    std::string toFullKind(const std::string& kind, std::map<std::string, std::vector<std::string>>& mapper) {
        size_t index = kind.find(".");
        if (index == std::string::npos) {
            if (mapper.count(kind) == 0) {
                std::cerr << "Wrong kind, please invoke 'GetKinds" << std::endl;
                return "";
            } else if (mapper[kind].size() == 1) {
                return mapper[kind][0];
            } else {
                std::string value = "";
                for (const auto& s : mapper[kind]) {
                    value += "," + s;
                }
                std::cerr <<"Please input fullKind: " + value.substr(1) << std::endl;
                return "";
            }
        }
        return kind;
    }


    std::string namespacePath(bool supportNS, const std::string& ns) {
        if (supportNS && !ns.empty()) {
            // if a kind supports namespace, and namespace is not empty
            return "namespaces/" + ns + "/";
        }
        return "";
    }

    std::string fullKind(const std::string& jsonObj) {
        std::string kind, apiVersion;
        try {
            auto parsedJson = nlohmann::json::parse(jsonObj);
            kind = parsedJson["kind"].get<std::string>();
            apiVersion = parsedJson["apiVersion"].get<std::string>();
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            return ""; 
        }

        size_t index = apiVersion.find('/');
        if (index == std::string::npos) {
            return kind;
        }
        return apiVersion.substr(0, index) + "." + kind;
    }

    
}

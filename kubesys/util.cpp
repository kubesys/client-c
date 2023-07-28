/*
 Copyright (2023, )  Institute of Software, Chinese Academy of Sciences
 Author:    lvxin22@otcaix.iscas.ac.cn
            wuheng@iscas.ac.cn
*/

#include "../include/util.h"
namespace kubesys {
    size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
        size_t totalSize = size * nmemb;
        response->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    }

    bool DoHttpRequest(CURL* curl_, const std::string& method, const std::string& url, const std::string& request,std::string& response) {
        //exist requestbody,set content-type
        if (!request.empty()) {
            // curl_slist* headers = nullptr;
            // headers = curl_slist_append(headers, "Content-Type: application/json");
            // curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, newHeaders);
            curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, request.c_str());
        }
        // std::cout << "DoHttpRequest url=" << url <<std::endl;
        curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, method.c_str());

        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &response);
        
        // curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1L);
        auto res = curl_easy_perform(curl_);
        if (res != CURLE_OK) {
            std::cerr << res << " HTTP request failed: " << curl_easy_strerror(res) << std::endl;
            return false;
        }
        return true;
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

    nlohmann::json ToJsonObject(const std::vector<uint8_t>& bytes) {
        std::string jsonStr(bytes.begin(), bytes.end());
        return nlohmann::json::parse(jsonStr);
    }

    std::map<std::string, nlohmann::json> ToGolangMap(const std::vector<uint8_t>& bytes) {
        std::string jsonStr(bytes.begin(), bytes.end());
        return nlohmann::json::parse(jsonStr).get<std::map<std::string, nlohmann::json>>();
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
                std::cout <<mapper[kind].size() <<std::endl;
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

    std::string fullKind(const nlohmann::json& parsedJson) {
        std::string kind, apiVersion;
        try {
            // auto parsedJson = nlohmann::json::parse(jsonObj);
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

    std::string getNamespace(const nlohmann::json& jsonObj) {
        if (jsonObj.contains("metadata") && jsonObj["metadata"].contains("namespace")) {
            return jsonObj["metadata"]["namespace"].get<std::string>();
        }
        return "";
    }
    
    std::string getName(const nlohmann::json& jsonObj) {
	    return jsonObj["metadata"]["name"].get<std::string>();
    }

    std::string kind(const std::string& fullKind) {
        size_t index = fullKind.find_last_of(".");
        if (index == std::string::npos) {
            return fullKind;
        }
        return fullKind.substr(index + 1);
    }
}

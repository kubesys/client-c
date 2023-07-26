/*
 Copyright (2023, )  Institute of Software, Chinese Academy of Sciences
 Author:    lvxin22@otcaix.iscas.ac.cn
            wuheng@iscas.ac.cn
*/

#include "../include/tls.h"

namespace kubesys {
Config* NewForConfig(const std::string& kubeConfig) {
    std::ifstream file(kubeConfig);
    if (!file.is_open()) {
        std::cerr << "Failed to open kubeconfig file." << std::endl;
        return nullptr;
    }

    Config* config = new Config;
    std::string line;
    int n = 0;
    while (std::getline(file, line)) {
        // line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty()) {
            continue;
        }
        else if (line.find("server:") != std::string::npos) {
            config->Server = line.substr(line.find("server:") + 1);
            n++;
        }
        else if (line.find("client-certificate-data:") != std::string::npos) {
            config->ClientCertificateData = line.substr(line.find("client-certificate-data:") + 1);
            n++;
        }
        else if (line.find("client-key-data:") != std::string::npos) {
            config->ClientKeyData = line.substr(line.find("client-key-data:") + 1);
            n++;
        }
        else if (line.find("certificate-authority-data:") != std::string::npos) {
            config->CertificateAuthorityData = line.substr(line.find(":") + 1);
            n++;
        }
    }
    file.close();

    if (n != 4) {
        delete config;
        std::cerr << "kubeconfig file error." << std::endl;
        return nullptr;
    }

    return config;
}
}
/*
int main() {
    std::string kubeConfig = "path_to_kubeconfig"; 
    Config* config = NewForConfig(kubeConfig);
    if (config) {
        std::cout << "Server: " << config->Server << std::endl;
        std::cout << "ClientCertificateData: " << config->ClientCertificateData << std::endl;
        std::cout << "ClientKeyData: " << config->ClientKeyData << std::endl;
        std::cout << "CertificateAuthorityData: " << config->CertificateAuthorityData << std::endl;

        delete config;
    }

    return 0;
}
*/
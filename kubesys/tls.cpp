/*
 Copyright (2023, )  Institute of Software, Chinese Academy of Sciences
 Author:    lvxin22@otcaix.iscas.ac.cn
            wuheng@iscas.ac.cn
*/

#include "../include/tls.h"
#include "../deps/cppcodec-master/cppcodec/base64_rfc4648.hpp"
#include <filesystem>
namespace kubesys {
struct Config NewForConfig(const std::string& kubeConfig) {
    struct Config config;
    std::ifstream file(kubeConfig);
    if (!file.is_open()) {
        std::cerr << "Failed to open kubeconfig file." << std::endl;
        return config;
    }
    std::string line;
    int n = 0;
    std::string server = "server:";
    std::string ccd = "client-certificate-data:";
    std::string ckd = "client-key-data:";
    std::string cad = "certificate-authority-data:";
    while (std::getline(file, line)) {
        // line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty()) {
            continue;
        }
        if (size_t  pos =line.find(server); pos != std::string::npos) {
            config.Server = line.substr(pos + server.size()+1);
            n++;
        }
        else if (size_t  pos =line.find(ccd); pos != std::string::npos) {
            auto certVector = cppcodec::base64_rfc4648::decode(line.substr(pos + ccd.size() + 1));
            std::string ca(certVector.begin(), certVector.end());
            config.ClientCertificateData = ca;
            n++;
        }
        else if (size_t  pos =line.find(ckd); pos != std::string::npos) {
            auto certVector = cppcodec::base64_rfc4648::decode(line.substr(pos + ckd.size() + 1));
            std::string key(certVector.begin(), certVector.end());
            config.ClientKeyData = key;
            n++;
        }
        else if (size_t  pos =line.find(cad); pos != std::string::npos) {
            auto certVector = cppcodec::base64_rfc4648::decode(line.substr(pos + cad.size() + 1));
            std::string pem(certVector.begin(), certVector.end());
            config.CertificateAuthorityData = pem;
            n++;
        }
    }
    file.close();
    if (n != 4) {
        std::cerr << "kubeconfig file error." << std::endl;
        return config;
    }
    return config;
}

std::string tlsFile(const std::string& name, const std::string& content) {
    std::string path = name;
    std::ofstream file;

    // 使用std::ios::trunc标志来打开文件，实现覆盖写入内容
    file.open(path, std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        file << content;
        file.close();
    } else {
        std::cerr << "Failed to create/open file: " << path << std::endl;
        return "";
    }
    path = (std::filesystem::current_path() / name).string();
    std::cout << "File path: " << path << std::endl;
    return path;
}
}

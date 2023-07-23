#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
namespace kubesys{
    struct Config {
        std::string Server;
        std::string ClientCertificateData;
        std::string ClientKeyData;
        std::string CertificateAuthorityData;
    };
    Config* NewForConfig(const std::string& kubeConfig);
}
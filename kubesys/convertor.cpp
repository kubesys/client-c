/*
 Copyright (2023, )  Institute of Software, Chinese Academy of Sciences
 Author:    lvxin22@otcaix.iscas.ac.cn
            wuheng@iscas.ac.cn
*/

#include "../include/client.h"
#include "../include/util.h"

namespace kubesys {
    auto KubernetesClient::BaseUrl(const std::string &fullKind, const std::string &nameSpace) -> std::string {
        std::string url = analyzer_->ruleBase_->FullKindToApiPrefixMapper[fullKind] + "/";
        url += namespacePath(analyzer_->ruleBase_->FullKindToNamespaceMapper[fullKind], nameSpace);
        url += analyzer_->ruleBase_->FullKindToNameMapper[fullKind];
        return url;
    }

    auto KubernetesClient::CreateResourceUrl(const std::string& fullKind, const std::string& nameSpace) -> std::string {
        return BaseUrl(fullKind, nameSpace);
    }

    auto KubernetesClient::ListResourcesUrl(const std::string& fullKind, const std::string& nameSpace) -> std::string {
        return BaseUrl(fullKind, nameSpace);
    }

    auto KubernetesClient::UpdateResourceUrl(const std::string& fullKind, const std::string& nameSpace, const std::string& name) -> std::string {
        return BaseUrl(fullKind, nameSpace) + "/" + name;
    }

    auto KubernetesClient::DeleteResourceUrl(const std::string& fullKind, const std::string& nameSpace, const std::string& name) -> std::string {
        return BaseUrl(fullKind, nameSpace) + "/" + name;
    }

    auto KubernetesClient::GetResourceUrl(const std::string& fullKind, const std::string& nameSpace, const std::string& name) -> std::string {
        return BaseUrl(fullKind, nameSpace) + "/" + name;
    }

    auto KubernetesClient::UpdateResourceStatusUrl(const std::string& fullKind, const std::string& nameSpace, const std::string& name) -> std::string {
        return BaseUrl(fullKind, nameSpace) + "/" + name + "/status";
    }

    auto KubernetesClient::BindingResourceStatusUrl(const std::string& fullKind, const std::string& nameSpace, const std::string& name) -> std::string {
        return BaseUrl(fullKind, nameSpace) + "/" + name + "/binding";
    }


}

#pragma once
#include <functional>
// #include <future>
#include <map>
#include <memory>
#include <string>
#include <cpr.h>

#include "data-types.h"
#include "requests-manager.h"

namespace vk_api {

class CommunicationInterface {
public:
    typedef std::function<std::string(const std::string& auth_url)> TokenRetrieve;

    CommunicationInterface(const std::string& client_id, const TokenRetrieve& callback);

    void UpdateToken();
    RequestsManager::Response SendRequest(const std::string& interface, const std::string& method,
                                          cpr::Parameters&& params);

private:
    const std::string client_id_;
    const TokenRetrieve token_retrieve_callback_;
    RequestsManager requests_manager_;
    std::string access_token_;

    std::string GetAuthUrl() const;
    cpr::Url GetUrl(const std::string& method_name, cpr::Parameters* params);

};

}

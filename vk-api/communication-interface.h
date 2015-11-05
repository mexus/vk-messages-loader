#pragma once
#include <functional>
// #include <future>
#include <map>
#include <memory>
#include <string>
#include <cpr.h>

#include "requests-manager.h"

namespace vk_api {

class CommunicationInterface {
public:
    typedef std::function<std::string(const std::string& auth_url)> TokenRetrieve;
    typedef std::unique_ptr<rapidjson::Document> Response;

    CommunicationInterface(const std::string& client_id, const TokenRetrieve& callback, RequestsManager* requests_manager);

    void UpdateToken();
    Response SendRequest(const std::string& interface, const std::string& method, cpr::Parameters&& params);

private:
    const std::string client_id_;
    const TokenRetrieve token_retrieve_callback_;
    RequestsManager * const requests_manager_;
    std::string access_token_;

    std::string GetAuthUrl() const;
    cpr::Url GetUrl(const std::string& method_name, cpr::Parameters* params);

    bool BreakOnError(const rapidjson::Value& error);
};

}

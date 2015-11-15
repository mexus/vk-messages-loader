#include <vk-api/communication-interface.h>
#include <exception>
#include <sstream>
#include <vector>
#include <iostream>

#include <cpr.h>
#include <manager/exceptions.h>
#include <vk-api/data-types.h>

namespace vk_api {

CommunicationInterface::CommunicationInterface(const TokenRetrieve& callback)
        : token_retrieve_callback_(callback) {
    if (!callback)
        throw std::logic_error("Callback function should not be null");
}

void CommunicationInterface::UpdateToken() {
    std::string token = token_retrieve_callback_();
    if (token.empty()) {
        THROW_AT(manager::NoTokenException);
    }
    access_token_ = token;
}

RequestsManager::Response CommunicationInterface::SendRequest(const std::string& interface, const std::string& method, cpr::Parameters&& params) {
    cpr::Url url = GetUrl(interface + "." + method, &params);
    auto doc = requests_manager_.MakeRequest(url, params);
    if (!doc.HasMember("response")) {
        THROW_AT(util::json::NoFieldException, "response");
    }
    return doc;
}

std::string CommunicationInterface::GetUrl(const std::string& method_name, cpr::Parameters* params) {
    static const std::string
        address("https://api.vk.com/method/"),
        version("5.37");
    if (access_token_.empty())
        UpdateToken();
    cpr::Url url = address + method_name;
    params->AddParameter({"access_token", access_token_});
    params->AddParameter({"v", version});
    params->AddParameter({"test_mode", "1"});
    return url;
}

}

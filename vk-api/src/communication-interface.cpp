#include <vk-api/communication-interface.h>
#include <exception>
#include <sstream>
#include <vector>
#include <iostream>

#include <cpr.h>
#include <vk-api/data-types.h>

namespace vk_api {

CommunicationInterface::CommunicationInterface(const std::string& client_id, const TokenRetrieve& callback)
        : client_id_(client_id),
          token_retrieve_callback_(callback) {
    if (!callback)
        throw std::logic_error("Callback function should not be null");
}

std::string CommunicationInterface::GetAuthUrl() const {
    static const std::string
        address("https://oauth.vk.com/authorize"),
        redirect_url("https://oauth.vk.com/blank.html"),
        display("page"),
        response_type("token"),
        version("5.37"),
        scope("messages,friends");
    cpr::Parameters params;
    params.AddParameter({"display", display});
    params.AddParameter({"redirect_url", redirect_url});
    params.AddParameter({"response_type", response_type});
    params.AddParameter({"v", version});
    params.AddParameter({"scope", scope});
    params.AddParameter({"client_id", client_id_});
    return address + "?" + params.content;
}

void CommunicationInterface::UpdateToken() {
    std::string token = token_retrieve_callback_(GetAuthUrl());
    if (token.empty())
        throw std::runtime_error("Empty token received");
    access_token_ = token;
}

RequestsManager::Response CommunicationInterface::SendRequest(const std::string& interface, const std::string& method, cpr::Parameters&& params) {
    cpr::Url url = GetUrl(interface + "." + method, &params);
    auto reply = requests_manager_.MakeRequest(url, params);
    if (reply.error.status == Error::VK_ERROR) {
        std::cerr << "Received VK error\n";
        return reply;
    }
    auto &doc = reply.json;
    if (!doc.HasMember("error") && !doc.HasMember("response")) {
        std::cerr << "Reply from the server is mallformed: there is neither no erorr, nor response field\n";
        return {};
    }
    return reply;
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

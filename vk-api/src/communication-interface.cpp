#include <vk-api/communication-interface.h>
#include <exception>
#include <sstream>
#include <vector>
#include <iostream>

#include <cpr.h>

namespace {

enum HTTP_CODES {
    OK = 200
};

enum VK_ERRORS {
    UNKNOWN = 1,
    APP_IS_OFF = 2,
    UNKNOWN_METHOD = 3,
    WRONG_SIGNATURE = 4,
    AUTHORIZATION_FAILED = 5,
    TOO_FAST_QUERIES = 6,
    PERMISSION_DENIED = 7,
    INCORRECT_QUERY = 8,
    TOO_MANY_SIMILAR_QUERIES = 9,
    INTERNAL_ERROR = 10,
    TEST_APPLICATION = 11,
    CAPTURE_REQUIRED = 14,
    ACCESS_DENIED = 15,
    HTTPS_REQUIRED = 16,
    VALIDATION_REQUIRED = 17,
    METHOD_DISABLER = 23,
    USER_CONFIRMATION_REQUIRED = 24,
    PARAMETER_ERROR = 100,
    WRONG_APP_ID = 101,
    WRONG_USER_ID = 113,
    WRONG_TIMESTAMP = 150
};

}

namespace vk_api {

CommunicationInterface::CommunicationInterface(const std::string& client_id, const TokenRetrieve& callback, RequestsManager* requests_manager)
        : client_id_(client_id),
          token_retrieve_callback_(callback),
          requests_manager_(requests_manager) {
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

CommunicationInterface::Response CommunicationInterface::SendRequest(const std::string& interface, const std::string& method, cpr::Parameters&& params) {
    cpr::Url url = GetUrl(interface + "." + method, &params);
    auto reply = requests_manager_->MakeRequest(url, params);
    if (!reply) {
        std::cerr << "Failed to receive a reply\n";
        return {};
    }
    auto end = reply->MemberEnd();
    auto error_it = reply->FindMember("error");
    if (error_it == end && !reply->HasMember("response")) {
        std::cerr << "Reply from the server is mallformed: there is neither no erorr, nor response field\n";
        return {};
    }
    if (error_it != end && BreakOnError(error_it->value)) {
        return {};
    }
    return reply;
}

bool CommunicationInterface::BreakOnError(const rapidjson::Value& error) {
    if (!error.IsObject()) {
        std::cerr << "Error is not an object\n";
        return true;
    }
    auto error_code_it = error.FindMember("error_code");
    auto end = error.MemberEnd();
    if (error_code_it == end) {
        std::cerr << "Can't find error code\n";
        return true;
    }
    if (!error_code_it->value.IsInt()) {
        std::cerr << "Error code is not an integer\n";
        return true;
    }
    int error_code = error.GetInt();
    if (error_code == INTERNAL_ERROR) {
        std::cerr << "Received an internal error. This may be caused by incorrect access_token, among the other reasons\n";
        return true;
    }
    return false;
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

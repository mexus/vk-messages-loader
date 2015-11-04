#include <vk-api/api.h>
#include <exception>
#include <sstream>
#include <vector>
#include <iostream>

#include <cpr.h>

namespace {

enum HTTP_CODES {
    OK = 200
};

}

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
    static const std::string url_base = address +
        "?display=" + display +
        "&redirect_url=" + redirect_url +
        "&response_type=" + response_type +
        "&v=" + version +
        "&scope=" + scope ;
    return url_base + "&client_id=" + client_id_;
}

void CommunicationInterface::UpdateToken() {
    std::string token = token_retrieve_callback_(GetAuthUrl());
    if (token.empty())
        throw std::runtime_error("Empty token received");
    access_token_ = token;
}

// std::future<CommunicationInterface::Response> CommunicationInterface::SendRequestAsync(Request&& request) {
//     return std::async(std::launch::async,
//                       &CommunicationInterface::SendRequest,
//                       this,
//                       std::move(request));
// }

CommunicationInterface::Response CommunicationInterface::SendRequest(Request&& request) {
    std::string url = GetUrl(request);
    cpr::Parameters params;
    for (auto& param: request.names_and_values) {
        auto& name = param.first;
        auto& value = param.second;
        params.AddParameter({name, value});
    }
    auto http_response = cpr::Get(cpr::Url(url), params);
    std::cout << "Processing result from " << http_response.url << "\n";
    if (http_response.status_code == HTTP_CODES::OK) {
        auto json = http_response.text;
        std::unique_ptr<rapidjson::Document> doc(new rapidjson::Document());
        std::cout << "Received a reply: [" << json << "]\n";
        doc->Parse(json.c_str());
        if (doc->HasParseError()) {
            std::cerr << "Can't parse response, parse error #" << doc->GetParseError()
                      << "at" << doc->GetErrorOffset() << ":\n" << json << "\n";
            return {};
        }
        if (!doc->IsObject()) {
            std::cerr << "Received json is not an object\n";
            return {};
        }
        // TODO
        // Test what is returned if the token is wrong/expired
        // and call for `token_retrieve_callback_`
        return doc;
    }
    std::cerr << "Received a http error code: " << http_response.status_code << "\n";
    std::cerr << http_response.url << "\n";
    std::cerr << http_response.text << "\n";
    return {};
}


std::string CommunicationInterface::GetUrl(const Request& request) {
    static const std::string
        address("https://api.vk.com/method/"),
        version("5.37");
    if (access_token_.empty())
        UpdateToken();
    std::string url = address + request.method_name + "?access_token=" + access_token_ + "&v=" + version;
    for (auto& param: request.names_and_values) {
        auto& name = param.first;
        auto& value = param.second;
        url += "&" + name + "=" + value;
    }
    return url;
}

}

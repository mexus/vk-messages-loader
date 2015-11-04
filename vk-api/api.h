#pragma once
#include <functional>
// #include <future>
#include <map>
#include <memory>
#include <string>

#include <rapidjson/document.h>

namespace vk_api {

struct Request {
    std::string method_name;
    std::map<std::string, std::string> names_and_values;
};

class CommunicationInterface {
public:
    typedef std::function<std::string(const std::string& auth_url)> TokenRetrieve;
    typedef std::unique_ptr<rapidjson::Document> Response;

    CommunicationInterface(const std::string& client_id, const TokenRetrieve& callback);

    void UpdateToken();
    // std::future<Response> SendRequestAsync(Request&& request);
    Response SendRequest(Request&& request);

private:
    const std::string client_id_;
    const TokenRetrieve token_retrieve_callback_;
    std::string access_token_;

    std::string GetAuthUrl() const;
    std::string GetUrl(const Request& request);
};

}

#include <vk-api/exceptions.h>

namespace vk_api {

ApiException::ApiException(VkError&& vk_error)
        : util::BasicException("Received a VK error #" + std::to_string(vk_error.error_code)),
          vk_error_(vk_error) {
}

const VkError& ApiException::GetVkError() const {
    return vk_error_;
}

RequestException::RequestException(const cpr::Response& response)
        : util::BasicException("Received a http code #" + std::to_string(response.status_code) + " on url " +
                             response.url + " with a text: " + response.text),
          url_(response.url),
          status_code_(response.status_code),
          text_(response.text) {
}

RequestException::RequestException(const cpr::Response& response, const std::string& message)
        : util::BasicException("Received a http code #" + std::to_string(response.status_code) + " on url " +
                             response.url + " with a message '" + message + "' and a text: " + response.text),
          url_(response.url),
          status_code_(response.status_code),
          text_(response.text) {
}

std::string RequestException::GetUrl() const {
    return url_;
}

long RequestException::GetStatusCode() const {
    return status_code_;
}

std::string RequestException::GetText() const {
    return text_;
}

RequestParseException::RequestParseException(const cpr::Response& response, const rapidjson::Document& doc)
        : RequestException(response, "Parse error"),
          error_code_(doc.GetParseError()),
          error_offset_(doc.GetErrorOffset()) {
}

rapidjson::ParseErrorCode RequestParseException::GetErrorCode() const {
    return error_code_;
}

size_t RequestParseException::GetErrorOffset() const {
    return error_offset_;
}

}

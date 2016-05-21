#include <vk-api/exceptions.h>

namespace vk_api {

ApiException::ApiException(const std::string& at, VkError&& vk_error)
    : util::BasicException(
          at, "Received a VK error #" + std::to_string(vk_error.error_code)),
      vk_error_(vk_error) {}

const VkError& ApiException::GetVkError() const { return vk_error_; }

RequestException::RequestException(const std::string& at,
                                   const cpr::Response& response)
    : util::BasicException(
          at, "Received a http code #" + std::to_string(response.status_code) +
                  " on url " + response.url + " with a text: " + response.text),
      url_(response.url),
      status_code_(response.status_code),
      text_(response.text) {}

RequestException::RequestException(const std::string& at,
                                   const cpr::Response& response,
                                   const std::string& message)
    : util::BasicException(
          at, "Received a http code #" + std::to_string(response.status_code) +
                  " on url " + response.url + " with a message '" + message +
                  "' and a text: " + response.text),
      url_(response.url),
      status_code_(response.status_code),
      text_(response.text) {}

std::string RequestException::GetUrl() const { return url_; }

long RequestException::GetStatusCode() const { return status_code_; }

std::string RequestException::GetText() const { return text_; }

RequestParseException::RequestParseException(const std::string& at,
                                             const cpr::Response& response,
                                             const rapidjson::Document& doc)
    : RequestException(at, response, "Parse error"),
      error_code_(doc.GetParseError()),
      error_offset_(doc.GetErrorOffset()) {}

rapidjson::ParseErrorCode RequestParseException::GetErrorCode() const {
  return error_code_;
}

size_t RequestParseException::GetErrorOffset() const { return error_offset_; }

CaptchaException::CaptchaException(const std::string& at, bool no_id,
                                   bool no_img)
    : util::BasicException(at, "Captcha parameters are missing"),
      no_id_(no_id),
      no_img_(no_img) {}

bool CaptchaException::NoId() const { return no_id_; }

bool CaptchaException::NoImage() const { return no_img_; }
}

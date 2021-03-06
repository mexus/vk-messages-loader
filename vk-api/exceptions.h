#pragma once

#include <cpr.h>
#include <utils/exceptions.h>
#include "data-types.h"

namespace vk_api {

class ApiException : public util::BasicException {
 public:
  ApiException(const std::string& at, VkError&& vk_error);
  const VkError& GetVkError() const;

 private:
  const VkError vk_error_;
};

class RequestException : public util::BasicException {
 public:
  RequestException(const std::string& at, const cpr::Response& response);
  RequestException(const std::string& at, const cpr::Response& response,
                   const std::string& message);
  std::string GetUrl() const;
  long GetStatusCode() const;
  std::string GetText() const;

 private:
  const std::string url_;
  const long status_code_;
  const std::string text_;
  const std::string message_;
};

class RequestParseException : public RequestException {
 public:
  RequestParseException(const std::string& at, const cpr::Response& response,
                        const rapidjson::Document& doc);
  rapidjson::ParseErrorCode GetErrorCode() const;
  size_t GetErrorOffset() const;

 private:
  const rapidjson::ParseErrorCode error_code_;
  const size_t error_offset_;
};

class CaptchaException : public util::BasicException {
 public:
  CaptchaException(const std::string& at, bool no_id, bool no_img);

  bool NoId() const;
  bool NoImage() const;

 private:
  bool no_id_;
  bool no_img_;
};
}

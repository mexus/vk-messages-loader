#pragma once
#include <cpr.h>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "callbacks.h"
#include "data-types.h"
#include "requests-manager.h"

#undef interface

namespace vk_api {

class CommunicationInterface {
 public:
  CommunicationInterface(Callbacks* callbacks);

  RequestsManager::Response SendRequest(const std::string& interface,
                                        const std::string& method,
                                        const cpr::Parameters& params);

 private:
  struct CaptchaData {
    std::string id;
    std::string key;
  };

  Callbacks* callbacks_;
  RequestsManager requests_manager_;
  CaptchaData captcha_data_;

  cpr::Parameters AddCaptcha(const cpr::Parameters& params);
  cpr::Url GetUrl(const std::string& method_name, cpr::Parameters* params);
  bool HandleVkError(const VkError& vk_error);
  void HandleCaptcha(const std::map<std::string, std::string>& data);
};
}

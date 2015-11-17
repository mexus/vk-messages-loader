#pragma once
#include <string>

namespace vk_api {

class Callbacks {
public:
    virtual ~Callbacks() = default;

    virtual std::string GetToken() = 0;
    virtual std::string GetCaptcha(const std::string& id, const std::string& img_url) = 0;
};

}

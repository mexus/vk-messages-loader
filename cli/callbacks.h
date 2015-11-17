#pragma once
#include <vk-api/callbacks.h>
#include "token.h"

namespace cli {

class Callbacks: public vk_api::Callbacks {
public:
    Callbacks(const std::string& app_id, const std::string& token_file);

    std::string GetToken() override;
    std::string GetCaptcha(const std::string& id, const std::string& img_url) override;

private:
    Token token_;
};

}

#pragma once
#include <manager/token.h>

namespace cli {

class Token: public manager::Token {
public:
    Token(const std::string& app_id, const std::string& file_name);

protected:
    virtual std::string GetTokenUrl(const std::string& auth_url) override;
};

}

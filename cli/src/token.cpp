#include <cli/token.h>

namespace cli {

Token::Token(const std::string& app_id, const std::string& file_name)
        : manager::Token(app_id, file_name) {
}

std::string Token::GetTokenUrl(const std::string& auth_url) {
    std::cout << "Visit " << auth_url << " and paste URL to which you will be redirected here\n";
    std::string url;
    std::cin >> url;
    LOG(DEBUG) << "Received url: [" << url << "]";
    return url;
}

}

#include <cli/callbacks.h>

namespace cli {

Callbacks::Callbacks(const std::string& app_id, const std::string& token_file)
    : token_(app_id, token_file) {}

std::string Callbacks::GetToken() { return token_.GetToken(); }

std::string Callbacks::GetCaptcha(const std::string& /*id*/,
                                  const std::string& img_url) {
  std::cout << "Open a captcha image: " << img_url << "\n"
            << "Enter the captcha: ";
  std::string captcha;
  std::cin >> captcha;
  return captcha;
}
}

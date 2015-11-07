#include <manager/token.h>
#include <iostream>
#include <fstream>
#include <cpr.h>
#include <utils/json.h>

namespace util {

template<>
bool JsonToObject<manager::Token::Data>(const rapidjson::Value& json, manager::Token::Data* object) {
    bool res = JsonGetMembers(json,
                              "access_token", &object->access_token,
                              "expire_at", &object->expire_at);
    if (!res) {
        std::cerr << "Can't extract token data from json\n";
    }
    return res;
}

template<>
rapidjson::Value JsonFromObject<manager::Token::Data>(const manager::Token::Data& object, JsonAllocator& allocator){
    rapidjson::Value json(rapidjson::kObjectType);
    JsonAddMembers(&json, allocator,
                   "access_token", object.access_token,
                   "expire_at", object.expire_at);
    return json;
}

}

namespace manager {

const std::string Token::kFieldName = "token";

Token::Token(const std::string& app_id, const std::string& file_name)
        : app_id_(app_id),
          file_name_(file_name),
          data_{"", 0} {
    LoadData();
}

void Token::LoadData() {
    auto doc = util::JsonFromFile(file_name_);
    if (!doc.IsObject()) {
        std::cerr << "Json document is not an object\n";
        return ;
    }
    if (!util::JsonGetMember(doc, kFieldName, &data_)) {
        std::cerr << "Can't extract token data from a json\n";
        return ;
    }
}

void Token::SaveData() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();
    util::JsonAddMember(&doc, kFieldName, data_, allocator);
    if (!util::JsonToFile(file_name_, doc)) {
        std::cerr << "Unable to save token data\n";
    }
}

std::string Token::GetToken() {
    static const time_t reserve_seconds = 3600;

    if (data_.access_token.empty() || data_.expire_at <= std::time(nullptr) + reserve_seconds) {
        auto token = ObtainToken();
        if (!token.access_token.empty()) {
            data_ = token;
            SaveData();
        }
    }
    return data_.access_token;
}

Token::Data Token::ObtainToken() {
    std::string auth_url = GetAuthUrl();
    std::cout << "Visit " << auth_url << " and paste URL to which you will be redirected here\n";
    std::string url;
    std::cin >> url;
    std::cout << "Processing [" << url << "]\n";

    static const std::string needle = "access_token=";
    size_t begin = url.find(needle);
    if (begin == std::string::npos) {
        std::cerr << "Can't find 'access_token'\n";
        return {};
    }
    begin += needle.length();
    size_t end = url.find('&', begin);
    if (end == std::string::npos)
        end = url.length();
    std::string token = url.substr(begin, end - begin);
    std::cout << "Token is: [" << token << "]\n";

    // FIXME
    // Extract expiration data from URI (expires_in=...)
    Data data{token, std::time(nullptr) + 86400};
    return data;
}

std::string Token::GetAuthUrl() const {
    static const std::string
        address("https://oauth.vk.com/authorize"),
        redirect_url("https://oauth.vk.com/blank.html"),
        display("page"),
        response_type("token"),
        version("5.37"),
        scope("messages,friends");
    cpr::Parameters params;
    params.AddParameter({"display", display});
    params.AddParameter({"redirect_url", redirect_url});
    params.AddParameter({"response_type", response_type});
    params.AddParameter({"v", version});
    params.AddParameter({"scope", scope});
    params.AddParameter({"client_id", app_id_});
    return address + "?" + params.content;
}

}

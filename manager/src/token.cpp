#include <manager/token.h>
#include <iostream>
#include <fstream>
#include <cpr.h>
#include <utils/json.h>

namespace util {

template<>
void JsonToObject<manager::Token::Data>(const rapidjson::Value& json, manager::Token::Data* object) {
    JsonGetMembers(json,
                   "access_token", &object->access_token,
                   "expire_at", &object->expire_at);
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
    try {
        LoadData();
    } catch (const util::FileReadException& e) {
        std::cout << "Can't load token data from " << e.GetFileName() << "\n";
    }
}

void Token::LoadData() {
    auto doc = util::JsonFromFile(file_name_);
    if (!doc.IsObject()) {
        std::cerr << "Json document is not an object\n";
        return ;
    }
    util::JsonGetMember(doc, kFieldName, &data_);
}

void Token::SaveData() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();
    util::JsonAddMember(&doc, kFieldName, data_, allocator);
    util::JsonToFile(file_name_, doc);
}

std::string Token::GetToken() {
    static const time_t reserve_seconds = 3600;

    if (data_.access_token.empty() || data_.expire_at <= std::time(nullptr) + reserve_seconds) {
        auto token = ObtainToken();
        if (!token.access_token.empty()) {
            data_ = token;
            try {
                SaveData();
            } catch (const std::runtime_error& e) {
                std::cerr << "Caught a runtime error while saving the data: " << e.what() << "\n";
            }
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
    if (app_id_.empty()) {
        throw std::runtime_error("No application id information available");
    }
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

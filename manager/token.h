#pragma once
#include <string>
#include <utils/json.h>

namespace manager {

class Token {
public:
    Token(const std::string& app_id, const std::string& file_name);
    virtual ~Token() = default;

    std::string GetToken();

protected:
    virtual std::string GetTokenUrl(const std::string& auth_url) = 0;

private:
    static const std::string kFieldName;
    struct Data {
        std::string access_token;
        time_t expire_at;
    };

    const std::string app_id_;
    const std::string file_name_;
    Data data_;

    std::string GetAuthUrl() const;
    Data ObtainToken();
    void LoadData();
    void SaveData() const;

    static std::string ExtractValue(const std::string& url, const std::string& parameter);
    static Data ProcessUrl(const std::string& url);

    friend void util::JsonToObject<Data>(const rapidjson::Value& json, Data* object);
    friend rapidjson::Value util::JsonFromObject<Data>(const Data& object, util::JsonAllocator& allocator);
};

}

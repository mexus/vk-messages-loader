#pragma once
#include <string>

namespace manager {

class Token {
public:
    struct Data {
        std::string access_token;
        time_t expire_at;
    };

    Token(const std::string& app_id, const std::string& file_name);

    std::string GetToken();

protected:
    std::string GetAuthUrl() const;
    virtual Data ObtainToken();

private:
    static const std::string kFieldName;

    const std::string app_id_;
    const std::string file_name_;
    Data data_;

    void LoadData();
    void SaveData() const;
};

}

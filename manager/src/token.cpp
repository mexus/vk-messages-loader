#include <fstream>
#include <iostream>

#include <cpr.h>

#include <manager/exceptions.h>
#include <manager/token.h>
#include <utils/json.h>

namespace util {

template <>
void JsonToObject<>(const rapidjson::Value& json,
                    manager::Token::Data* object) {
  JsonGetMembers(json)("access_token", &object->access_token)(
      "expire_at", &object->expire_at);
}

template <>
rapidjson::Value JsonFromObject<>(const manager::Token::Data& object,
                                  JsonAllocator& allocator) {
  rapidjson::Value json(rapidjson::kObjectType);
  JsonAddMembers(&json, allocator, "access_token", object.access_token,
                 "expire_at", object.expire_at);
  return json;
}
}

namespace manager {

const std::string Token::kFieldName = "token";

Token::Token(const std::string& app_id, const std::string& file_name)
    : app_id_(app_id), file_name_(file_name), data_{"", 0} {
  try {
    LoadData();
  } catch (const util::FileReadException& e) {
    LOG(ERROR) << "Can't load token data from " << e.GetFileName();
  } catch (const util::json::Exception& e) {
    LOG(ERROR) << "Json parsing of '" << file_name << "' failed at `"
               << e.GetAt() << "`: " << e.GetText();
  }
}

void Token::LoadData() {
  auto doc = util::JsonFromFile(file_name_);
  util::JsonGetMember(doc, kFieldName, &data_);
}

void Token::SaveData() const {
  rapidjson::Document doc;
  doc.SetObject();
  auto& allocator = doc.GetAllocator();
  try {
    util::JsonAddMember(&doc, kFieldName, data_, allocator);
    util::JsonToFile(file_name_, doc);
  } catch (const util::BasicException& e) {
    LOG(ERROR) << "Caught a basic exception at `" << e.GetAt() << "` "
               << "while saving the data: " << e.GetText();
  }
}

std::string Token::GetToken() {
  static const time_t reserve_seconds = 3600;

  if (data_.access_token.empty() ||
      data_.expire_at <= std::time(nullptr) + reserve_seconds) {
    auto temp = ObtainToken();
    if (temp.access_token.empty()) {
      THROW_AT(NoTokenException);
    }
    data_ = std::move(temp);
    SaveData();
  }
  return data_.access_token;
}

std::string Token::ExtractValue(const std::string& url,
                                const std::string& parameter) {
  size_t begin = url.find(parameter);
  if (begin == std::string::npos) {
    LOG(WARNING) << "Can't find '" << parameter << "' parameter in url `" << url
                 << "`";
    return {};
  }
  begin += parameter.length();
  size_t end = url.find('&', begin);
  if (end == std::string::npos) {
    end = url.length();
  }
  return url.substr(begin, end - begin);
}

Token::Data Token::ProcessUrl(const std::string& url) {
  std::string token = ExtractValue(url, "access_token=");
  LOG(DEBUG) << "Token is: [" << token << "]";

  unsigned int duration_sec = 86400;
  std::string duration = ExtractValue(url, "expires_in=");
  if (!duration.empty()) {
    duration_sec = std::stoul(duration);
  }
  Data data{token, std::time(nullptr) + duration_sec};
  return data;
}

Token::Data Token::ObtainToken() {
  std::string token_url = GetTokenUrl(GetAuthUrl());
  return ProcessUrl(token_url);
}

std::string Token::GetAuthUrl() const {
  if (app_id_.empty()) {
    THROW_AT(NoApplicationIdException);
  }
  static const std::string address("https://oauth.vk.com/authorize"),
      redirect_url("https://oauth.vk.com/blank.html"), display("page"),
      response_type("token"), version("5.37"), scope("messages,friends");
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

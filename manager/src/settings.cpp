#include <manager/settings.h>

namespace util {

template <>
void JsonToObject<>(const rapidjson::Value& json, manager::Settings* object) {
  JsonGetMembers(json, "application_id", &object->application_id_,
                 "storage_path", &object->storage_path_, "users",
                 &object->users_, util::json::Optional{});
}

template <>
rapidjson::Value JsonFromObject<>(const manager::Settings& object,
                                  JsonAllocator& allocator) {
  rapidjson::Value json(rapidjson::kObjectType);
  JsonAddMembers(&json, allocator, "application_id", object.application_id_,
                 "storage_path", object.storage_path_, "users", object.users_);
  return json;
}
}

namespace manager {

Settings::Settings(const std::string& file_name, const std::string& field)
    : file_name_(file_name), json_field_(field) {
  try {
    auto doc = util::JsonFromFile(file_name);
    util::JsonGetMember(doc, json_field_, this);
  } catch (const util::FileReadException& e) {
    LOG(ERROR) << "Can't load settings from file " << e.GetFileName();
  }
}

Settings::~Settings() { SaveToFile(); }

std::string Settings::GetApplicationId() const { return application_id_; }

void Settings::SetApplicationId(const std::string& id) { application_id_ = id; }

std::string Settings::GetStoragePath() const { return storage_path_; }

void Settings::SetStoragePath(const std::string& path) { storage_path_ = path; }

const std::vector<uint64_t>& Settings::GetUsers() const { return users_; }

std::vector<uint64_t>& Settings::GetUsers() { return users_; }

void Settings::SaveToFile() const {
  rapidjson::Document doc;
  doc.SetObject();
  auto& allocator = doc.GetAllocator();
  try {
    util::JsonAddMember(&doc, json_field_, *this, allocator);
    util::JsonToFile(file_name_, doc);
  } catch (const util::BasicException& e) {
    LOG(ERROR) << "Caught an application exception at `" << e.GetAt() << "` "
               << "while saving settings: " << e.GetMessage();
  }
}
}

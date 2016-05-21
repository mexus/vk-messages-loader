#pragma once
#include <utils/json.h>

namespace manager {

class Settings {
 public:
  Settings(const std::string& file_name, const std::string& field);
  ~Settings();

  std::string GetApplicationId() const;
  void SetApplicationId(const std::string& id);

  std::string GetStoragePath() const;
  void SetStoragePath(const std::string& path);

  const std::vector<uint64_t>& GetUsers() const;
  std::vector<uint64_t>& GetUsers();

 private:
  const std::string file_name_;
  const std::string json_field_;

  std::string application_id_;
  std::string storage_path_;
  std::vector<uint64_t> users_;

  void SaveToFile() const;

  friend void util::JsonToObject<>(const rapidjson::Value& json,
                                   manager::Settings* object);
  friend rapidjson::Value util::JsonFromObject<>(
      const manager::Settings& object, util::JsonAllocator& allocator);
};
}

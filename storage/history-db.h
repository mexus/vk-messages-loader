#pragma once
#include <map>
#include <memory>
#include "history.h"

namespace history_check_path  {
	struct ok;
	struct file_exists;
	struct unable_create_path;
	struct get_storage;
	struct get_user;
}

namespace testing {
	class TestDB;
}

namespace storage {

class HistoryDB {
 public:
  HistoryDB(const std::string& path);

  std::shared_ptr<History> GetUser(uint64_t user_id);

 private:
  const std::string path_;
  std::map<uint64_t, std::weak_ptr<History>> users_history_;

  std::shared_ptr<History> GetStorage(uint64_t user_id);

  void CheckPath() const;
  static void CheckFile(const std::string& file_name);

  friend class testing::TestDB;
};
}

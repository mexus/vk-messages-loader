#define BOOST_TEST_MODULE history storage
#define BOOST_TEST_DYN_LINK
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iostream>

#define private public
#include <storage/history-db.h>
#undef private
#include <storage/exceptions.h>

INITIALIZE_EASYLOGGINGPP

namespace {

struct Dir {
  Dir() : path("temp-path") {
    if (boost::filesystem::exists(path)) {
      boost::filesystem::remove_all(path);
    }
    boost::filesystem::create_directories(path);
  }

  ~Dir() { boost::filesystem::remove_all(path); }

  void MakeReadOnly() {
    using boost::filesystem::perms;
    boost::filesystem::permissions(path,
                                   perms::remove_perms | perms::owner_write);
  }

  const boost::filesystem::path path;
};

struct File {
  File() : path("temp-file") {
    if (boost::filesystem::exists(path)) {
      boost::filesystem::remove_all(path);
    }
    boost::filesystem::ofstream f(path.string());
    if (!f) throw std::runtime_error("Can't create file");
  }

  ~File() { boost::filesystem::remove_all(path); }

  const boost::filesystem::path path;
};
}

using namespace storage;

BOOST_AUTO_TEST_SUITE(history_check_path)

BOOST_AUTO_TEST_CASE(ok) {
  Dir dir;
  HistoryDB storage((dir.path / "subpath").string());
  storage.CheckPath();
}

BOOST_AUTO_TEST_CASE(file_exists) {
  File file;
  HistoryDB storage(file.path.string());
  BOOST_CHECK_EXCEPTION(storage.CheckPath(), PathIsFileException,
                        [&file](const PathIsFileException& e) {
                          return e.GetPath() == file.path.string();
                        });
}

BOOST_AUTO_TEST_CASE(unable_create_path) {
  Dir dir;
  dir.MakeReadOnly();
  std::string path = (dir.path / "subpath").string();
  HistoryDB storage(path);
  using exception = boost::filesystem::filesystem_error;
  BOOST_CHECK_EXCEPTION(
      storage.CheckPath(), exception,
      [&path](const exception& e) { return e.path1().string() == path; });
}

BOOST_AUTO_TEST_CASE(get_storage) {
  Dir dir;
  HistoryDB storage(dir.path.string());
  storage.CheckPath();

  uint64_t user_id = 123456;
  auto user_file = dir.path / std::to_string(user_id);
  auto history = storage.GetStorage(user_id);
  BOOST_REQUIRE(!(!history));
  history.reset();
  BOOST_REQUIRE(boost::filesystem::exists(user_file) &&
                boost::filesystem::is_regular_file(user_file));
}

BOOST_AUTO_TEST_CASE(get_user) {
  Dir dir;
  HistoryDB storage(dir.path.string());
  storage.CheckPath();

  uint64_t user_id = 123456;
  auto user_file = dir.path / std::to_string(user_id);
  auto history = storage.GetUser(user_id);
  BOOST_REQUIRE(!(!history));
  history.reset();
  BOOST_REQUIRE(boost::filesystem::exists(user_file) &&
                boost::filesystem::is_regular_file(user_file));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(history_get_user) {}

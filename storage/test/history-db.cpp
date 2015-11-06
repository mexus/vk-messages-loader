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

namespace {

struct Dir {
    Dir() : path("temp-path") {
        if (boost::filesystem::exists(path)) {
            boost::filesystem::remove_all(path);
        }
        boost::filesystem::create_directories(path);
    }

    ~Dir() {
        boost::filesystem::remove_all(path);
    }

    void MakeReadOnly() {
        using boost::filesystem::perms;
        boost::filesystem::permissions(path, perms::remove_perms | perms::owner_write);
    }

    const boost::filesystem::path path;
};

struct File {
    File() : path("temp-file") {
        if (boost::filesystem::exists(path)) {
            boost::filesystem::remove_all(path);
        }
        boost::filesystem::ofstream f(path.string());
        if (!f)
            throw std::runtime_error("Can't create file");
    }

    ~File() {
        boost::filesystem::remove_all(path);
    }

    const boost::filesystem::path path;
};

}

using namespace storage;

BOOST_AUTO_TEST_SUITE(history_check_path)

BOOST_AUTO_TEST_CASE(ok) {
    Dir dir;
    HistoryDB storage((dir.path / "subpath").string());
    BOOST_REQUIRE(storage.CheckPath());
}

BOOST_AUTO_TEST_CASE(file_exists) {
    File file;
    HistoryDB storage(file.path.string());
    std::cout << "Expecting an error regarding " << file.path << std::endl;
    BOOST_REQUIRE(!storage.CheckPath());
}

BOOST_AUTO_TEST_CASE(unable_create_path) {
    Dir dir;
    dir.MakeReadOnly();
    auto path = dir.path / "subpath";
    HistoryDB storage(path.string());
    std::cout << "Expecting an error regarding " << path  << std::endl;
    BOOST_REQUIRE(!storage.CheckPath());
}

BOOST_AUTO_TEST_CASE(get_storage) {
    Dir dir;
    HistoryDB storage(dir.path.string());
    BOOST_REQUIRE(storage.CheckPath());

    uint64_t user_id = 123456;
    auto user_file = dir.path / std::to_string(user_id);
    auto history = storage.GetStorage(user_id);
    BOOST_REQUIRE(!(!history));
    BOOST_REQUIRE(boost::filesystem::exists(user_file) &&
                  boost::filesystem::is_regular_file(user_file));
}

BOOST_AUTO_TEST_CASE(get_user) {
    Dir dir;
    HistoryDB storage(dir.path.string());
    BOOST_REQUIRE(storage.CheckPath());

    uint64_t user_id = 123456;
    auto user_file = dir.path / std::to_string(user_id);
    auto history = storage.GetUser(user_id);
    BOOST_REQUIRE(!(!history));
    BOOST_REQUIRE(boost::filesystem::exists(user_file) &&
                  boost::filesystem::is_regular_file(user_file));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(history_get_user) {
}

#define BOOST_TEST_DYN_LINK
#include <storage/message.h>
#include <utils/json.h>
#include <boost/test/unit_test.hpp>
#include <limits>
#include <random>
#include "operators.h"

using namespace storage;

BOOST_AUTO_TEST_CASE(message_converters) {
  rapidjson::Document doc;
  auto& allocator = doc.GetAllocator();
  std::random_device rd;
  std::uniform_int_distribution<uint64_t> users_distr;
  std::mt19937_64 generator(rd());
  Message etalon_message{std::numeric_limits<uint64_t>::max(),
                         4602523593,
                         users_distr(generator),
                         users_distr(generator),
                         "test body",
                         {{PHOTO, "PICTURE"}, {VIDEO, "ahaha"}}};

  auto json = util::JsonFromObject(etalon_message, allocator);
  BOOST_REQUIRE(json.IsObject());

  Message result;
  util::JsonToObject(json, &result);
  BOOST_REQUIRE_EQUAL(etalon_message, result);
}

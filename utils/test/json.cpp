#define BOOST_TEST_MODULE utils
#define BOOST_TEST_DYN_LINK
#include <utils/json.h>
#include <boost/test/unit_test.hpp>
#include <limits>

using namespace util;
INITIALIZE_EASYLOGGINGPP

BOOST_AUTO_TEST_SUITE(utils_json)

BOOST_AUTO_TEST_CASE(string_conversion) {
  rapidjson::Document doc;
  auto& allocator = doc.GetAllocator();
  const std::string etalon = "Etalon \n";
  auto json = JsonFromObject(etalon, allocator);
  BOOST_REQUIRE(json.IsString());
  std::string result;
  JsonToObject(json, &result);
  BOOST_REQUIRE_EQUAL(etalon, result);
}

BOOST_AUTO_TEST_CASE(uint64_conversion) {
  rapidjson::Document doc;
  auto& allocator = doc.GetAllocator();
  const uint64_t etalon = std::numeric_limits<uint64_t>::max();
  auto json = JsonFromObject(etalon, allocator);
  BOOST_REQUIRE(json.IsUint64());
  uint64_t result;
  JsonToObject(json, &result);
  BOOST_REQUIRE_EQUAL(etalon, result);
}

BOOST_AUTO_TEST_CASE(time_conversion) {
  rapidjson::Document doc;
  auto& allocator = doc.GetAllocator();
  const time_t etalon = 4602511603;
  auto json = JsonFromObject(etalon, allocator);
  BOOST_REQUIRE(json.IsInt64());
  time_t result;
  JsonToObject(json, &result);
  BOOST_REQUIRE_EQUAL(etalon, result);
}

BOOST_AUTO_TEST_CASE(vector_string) {
  rapidjson::Document doc;
  auto& allocator = doc.GetAllocator();
  const std::vector<std::string> etalon{"a1", "a2", "\n\t\r"};
  auto json = JsonFromObject(etalon, allocator);
  BOOST_REQUIRE(json.IsArray());
  std::vector<std::string> result;
  JsonToObject(json, &result);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(etalon.begin(), etalon.end(), result.begin(),
                                  result.end());
}

BOOST_AUTO_TEST_CASE(string_member) {
  rapidjson::Document doc;
  doc.SetObject();
  auto& allocator = doc.GetAllocator();
  const std::string field("field1");
  const std::string etalon_value("value1");
  JsonAddMember(&doc, field, etalon_value, allocator);

  auto it = doc.FindMember(field.c_str());
  BOOST_REQUIRE(it != doc.MemberEnd());
  BOOST_REQUIRE(it->value.IsString());
  BOOST_REQUIRE_EQUAL(etalon_value, it->value.GetString());

  std::string result;
  JsonGetMember(doc, field, &result);
  BOOST_REQUIRE_EQUAL(etalon_value, result);
}

BOOST_AUTO_TEST_CASE(string_member_error) {
  rapidjson::Document doc;
  doc.SetObject();
  auto& allocator = doc.GetAllocator();
  const std::string field("field1");
  const std::string etalon_value("value1");
  JsonAddMember(&doc, field, etalon_value, allocator);

  auto it = doc.FindMember(field.c_str());
  BOOST_REQUIRE(it != doc.MemberEnd());
  BOOST_REQUIRE(it->value.IsString());
  BOOST_REQUIRE_EQUAL(etalon_value, it->value.GetString());

  it->value.SetString("WRONG VALUE");
  std::string result;
  JsonGetMember(doc, field, &result);
  BOOST_REQUIRE_NE(etalon_value, result);

  doc.EraseMember(it);
  BOOST_REQUIRE_EXCEPTION(JsonGetMember(doc, field, &result),
                          json::NoFieldException,
                          [&field](json::NoFieldException const& e) {
                            return e.GetField() == field;
                          });
}

BOOST_AUTO_TEST_CASE(
    stream_write, *boost::unit_test::depends_on("utils_json/string_member")) {
  rapidjson::Document doc;
  doc.SetObject();
  auto& allocator = doc.GetAllocator();

  const std::string field("field");
  const std::string etalon_value("va\nl");
  const std::string etalon_value_escaped("va\\nl");
  std::string etalon_json =
      "{\n"
      "    \"" +
      field + "\": \"" + etalon_value_escaped +
      "\"\n"
      "}";
  JsonAddMember(&doc, field, etalon_value, allocator);

  std::stringstream stream;
  JsonToStream(stream, doc);
  BOOST_REQUIRE_EQUAL(etalon_json, stream.str());
}

BOOST_AUTO_TEST_CASE(
    stream_read, *boost::unit_test::depends_on("utils_json/string_member")) {
  const std::string field("field");
  const std::string etalon_value("va\nl");
  const std::string etalon_value_escaped("va\\nl");
  std::string etalon_json =
      "{\"" + field + "\": \"" + etalon_value_escaped + "\"}";

  std::stringstream stream;
  stream << etalon_json;
  auto doc = JsonFromStream(stream);

  BOOST_REQUIRE(doc.IsObject());
  std::string result;
  JsonGetMember(doc, field, &result);
  BOOST_REQUIRE_EQUAL(etalon_value, result);
}

BOOST_AUTO_TEST_SUITE_END()

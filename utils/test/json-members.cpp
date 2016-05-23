#define BOOST_TEST_MODULE utils
#include <utils/json-members.h>
#include <boost/test/unit_test.hpp>
#include <limits>

using namespace util;

BOOST_AUTO_TEST_SUITE(utils_json)

BOOST_AUTO_TEST_CASE(string_members_chain) {
  rapidjson::Document doc;
  doc.SetObject();
  auto& allocator = doc.GetAllocator();
  const std::string field1("field1"), field2("aaa");
  const std::string etalon_value1("value1"), etalon_value2("\n");
  JsonMembers(doc, allocator)(field1, etalon_value1)(field2, etalon_value2);

  auto it1 = doc.FindMember(field1.c_str()),
       it2 = doc.FindMember(field2.c_str());
  BOOST_REQUIRE(it1 != doc.MemberEnd());
  BOOST_REQUIRE(it2 != doc.MemberEnd());
  BOOST_REQUIRE(it1->value.IsString());
  BOOST_REQUIRE(it2->value.IsString());
  BOOST_REQUIRE_EQUAL(etalon_value1, it1->value.GetString());
  BOOST_REQUIRE_EQUAL(etalon_value2, it2->value.GetString());

  std::string result1, result2;
  JsonMembers(doc)(field1, &result1)(field2, &result2);
  BOOST_REQUIRE_EQUAL(etalon_value1, result1);
  BOOST_REQUIRE_EQUAL(etalon_value2, result2);
}

BOOST_AUTO_TEST_CASE(get_members_optional) {
  const std::string field1("non-existent-field1");
  const std::string field2("non-existent-field2");
  rapidjson::Document doc;
  doc.SetObject();

  std::string result1, result2;
  JsonMembers(doc)(field1, &result1, json::Optional{})(
      field2, &result2, json::Optional{});
  BOOST_REQUIRE_EQUAL(result1, std::string());
  BOOST_REQUIRE_EQUAL(result2, std::string());
}

BOOST_AUTO_TEST_CASE(get_members_optional_mixed) {
  const std::string field1("non-existent-field1");
  const std::string field2("existent-field2");
  const std::string etalon_value("test");
  const std::string field3("non-existent-field2");
  rapidjson::Document doc;
  doc.SetObject();
  auto& allocator = doc.GetAllocator();
  JsonAddMember(&doc, field2, etalon_value, allocator);

  std::string result1, result2, result3;
  JsonMembers(doc)(field1, &result1, json::Optional{})(
      field2, &result2)(field3, &result3, json::Optional{});
  BOOST_REQUIRE_EQUAL(result1, std::string());
  BOOST_REQUIRE_EQUAL(result2, etalon_value);
  BOOST_REQUIRE_EQUAL(result3, std::string());
}

BOOST_AUTO_TEST_SUITE_END()

#define BOOST_TEST_MODULE utils
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <limits>
#include <utils/json.h>

using namespace util;

BOOST_AUTO_TEST_SUITE(utils_json)

BOOST_AUTO_TEST_CASE(string_conversion) {
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    const std::string etalon = "Etalon \n";
    auto json = JsonFromObject(etalon, allocator);
    BOOST_REQUIRE(json.IsString());
    std::string result;
    BOOST_REQUIRE(JsonToObject(json, &result));
    BOOST_REQUIRE_EQUAL(etalon, result);
}

BOOST_AUTO_TEST_CASE(uint64_conversion) {
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    const uint64_t etalon = std::numeric_limits<uint64_t>::max();
    auto json = JsonFromObject(etalon, allocator);
    BOOST_REQUIRE(json.IsUint64());
    uint64_t result;
    BOOST_REQUIRE(JsonToObject(json, &result));
    BOOST_REQUIRE_EQUAL(etalon, result);
}

BOOST_AUTO_TEST_CASE(time_conversion) {
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    const time_t etalon = 4602511603;
    auto json = JsonFromObject(etalon, allocator);
    BOOST_REQUIRE(json.IsInt64());
    time_t result;
    BOOST_REQUIRE(JsonToObject(json, &result));
    BOOST_REQUIRE_EQUAL(etalon, result);
}

BOOST_AUTO_TEST_CASE(vector_string) {
    rapidjson::Document doc;
    auto& allocator = doc.GetAllocator();
    const std::vector<std::string> etalon {"a1", "a2", "\n\t\r"};
    auto json = JsonFromObject(etalon, allocator);
    BOOST_REQUIRE(json.IsArray());
    std::vector<std::string> result;
    BOOST_REQUIRE(JsonToObject(json, &result));
    BOOST_REQUIRE_EQUAL_COLLECTIONS(etalon.begin(), etalon.end(),
                                    result.begin(), result.end());
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
    BOOST_REQUIRE(JsonGetMember(doc, field, &result));
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
    BOOST_REQUIRE(JsonGetMember(doc, field, &result));
    std::cout << "Expecting an error.." << std::endl;
    BOOST_REQUIRE_NE(etalon_value, result);

    doc.EraseMember(it);
    BOOST_REQUIRE(!JsonGetMember(doc, field, &result));
}

BOOST_AUTO_TEST_CASE(string_members_chain) {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();
    const std::string field1("field1"), field2("aaa");
    const std::string etalon_value1("value1"), etalon_value2("\n");
    JsonAddMembers(&doc, allocator,
                   field1, etalon_value1,
                   field2, etalon_value2);

    auto it1 = doc.FindMember(field1.c_str()),
         it2 = doc.FindMember(field2.c_str());
    BOOST_REQUIRE(it1 != doc.MemberEnd());
    BOOST_REQUIRE(it2 != doc.MemberEnd());
    BOOST_REQUIRE(it1->value.IsString());
    BOOST_REQUIRE(it2->value.IsString());
    BOOST_REQUIRE_EQUAL(etalon_value1, it1->value.GetString());
    BOOST_REQUIRE_EQUAL(etalon_value2, it2->value.GetString());

    std::string result1, result2;
    BOOST_REQUIRE(JsonGetMembers(doc,
                                 field1, &result1,
                                 field2, &result2));
    BOOST_REQUIRE_EQUAL(etalon_value1, result1);
    BOOST_REQUIRE_EQUAL(etalon_value2, result2);
}

BOOST_AUTO_TEST_CASE(stream_write, *boost::unit_test::depends_on("utils_json/string_member")) {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    const std::string field("field");
    const std::string etalon_value("va\nl");
    const std::string etalon_value_escaped("va\\nl");
    std::string etalon_json =
        "{\n"
        "    \"" + field + "\": \"" + etalon_value_escaped + "\"\n"
        "}";
    JsonAddMember(&doc, field, etalon_value, allocator);

    std::stringstream stream;
    util::JsonToStream(stream, doc);
    BOOST_REQUIRE_EQUAL(etalon_json, stream.str());
}

BOOST_AUTO_TEST_CASE(stream_read, *boost::unit_test::depends_on("utils_json/string_member")) {
    const std::string field("field");
    const std::string etalon_value("va\nl");
    const std::string etalon_value_escaped("va\\nl");
    std::string etalon_json = "{\"" + field + "\": \"" + etalon_value_escaped + "\"}";

    std::stringstream stream;
    stream << etalon_json;
    auto doc = JsonFromStream(stream);

    BOOST_REQUIRE(doc.IsObject());
    std::string result;
    BOOST_REQUIRE(JsonGetMember(doc, field, &result));
    BOOST_REQUIRE_EQUAL(etalon_value, result);
}

BOOST_AUTO_TEST_SUITE_END()
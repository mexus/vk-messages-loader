#define BOOST_TEST_DYN_LINK
#include <storage/history.h>
#include <boost/test/unit_test.hpp>
#include "operators.h"

using namespace storage;

BOOST_AUTO_TEST_SUITE(history)

BOOST_AUTO_TEST_CASE(messages_handling) {
  std::string file_name("fake-file-name");
  History history(file_name);
  const std::vector<Message> etalon_messages;
  for (auto& msg : etalon_messages) {
    history.AddMessage(Message(msg));
  }
  auto& result = history.GetData();
  BOOST_REQUIRE_EQUAL_COLLECTIONS(etalon_messages.begin(),
                                  etalon_messages.end(), result.begin(),
                                  result.end());
}

BOOST_AUTO_TEST_SUITE_END()

add_executable("utils-test-json"
        "json.cpp"
        "json-members.cpp"
)
target_link_libraries("utils-test-json" "utils-json" ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(utils-json "utils-test-json")

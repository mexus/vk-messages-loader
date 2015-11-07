add_executable("vk-api-test"
        "intervals-manager.cpp"
)
target_link_libraries("vk-api-test" "vk-api" ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(vk-api "vk-api-test")

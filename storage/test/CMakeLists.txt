add_executable("storage-test"
        "history-db.cpp"
        "history.cpp"
        "operators.cpp"
        "messages.cpp"
)
target_link_libraries("storage-test"
        "storage"
        "utils-json"
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
add_test(storage "storage-test")

SET(EXE_NAME "cli")

add_executable(${EXE_NAME}
               "main.cpp"
               "src/cli.cpp"
               "src/cli-exceptions-handler.cpp"
               "src/token.cpp"
               "src/callbacks.cpp"
)

target_link_libraries(${EXE_NAME}
        "manager"
        "vk-api"
        "utils-json"
        "storage"
        ${CPR_LIBRARIES}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

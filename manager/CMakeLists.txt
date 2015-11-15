add_library("manager"
            "src/cli-exceptions-handler.cpp"
            "src/exceptions.cpp"
            "src/export.cpp"
            "src/friends-cache.cpp"
            "src/manager.cpp"
            "src/settings.cpp"
            "src/token.cpp"
)

SET(EXE_NAME "cli")
add_executable(${EXE_NAME} "cli.cpp")
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

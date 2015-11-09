add_library("manager"
            "src/friends-cache.cpp"
            "src/token.cpp"
            "src/settings.cpp"
            "src/manager.cpp"
            "src/export.cpp"
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

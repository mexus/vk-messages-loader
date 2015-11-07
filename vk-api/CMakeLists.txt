add_library("vk-api"
    "src/communication-interface.cpp"
    "src/data-types.cpp"
    "src/friends.cpp"
    "src/intervals-manager.cpp"
    "src/messages.cpp"
    "src/requests-manager.cpp"
)

SET(EXE_NAME "binary")
add_executable(${EXE_NAME} "test.cpp")
target_link_libraries(${EXE_NAME} "vk-api" "utils-json" ${CPR_LIBRARIES})

add_subdirectory("test")

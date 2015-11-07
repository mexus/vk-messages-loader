add_library("manager"
            "src/friends-cache.cpp"
            "src/token.cpp"
            "src/settings.cpp"
            "src/manager.cpp"
)

SET(EXE_NAME "cli")
add_executable(${EXE_NAME} "manager.cpp")
target_link_libraries(${EXE_NAME} "vk-api" "utils-json" "storage" "manager" ${CPR_LIBRARIES})

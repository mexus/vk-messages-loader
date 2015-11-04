add_library("vk-api" "src/communication-interface.cpp")

SET(EXE_NAME "binary")
add_executable(${EXE_NAME} "test.cpp")
target_link_libraries(${EXE_NAME} "vk-api" ${CPR_LIBRARIES})

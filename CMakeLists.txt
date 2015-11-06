cmake_minimum_required (VERSION 3.1)
project("vk-message-loader")
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_EXTRA_FLAGS "-Wall -Wextra -Werror")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_EXTRA_FLAGS}")

include_directories("." "3rd-party/rapidjson/include")

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
enable_testing()

# Settings for "cpr":
SET(ENV{USE_SYSTEM_CURL} ON)
SET(ENV{BUILD_CPR_TESTS} OFF)

add_subdirectory("3rd-party/cpr")
include_directories(${CPR_INCLUDE_DIRS})
message(${CPR_INCLUDE_DIRS})

add_subdirectory("vk-api")
add_subdirectory("storage")
add_subdirectory("utils")

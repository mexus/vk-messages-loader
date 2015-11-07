add_library("vk-api"
    "src/communication-interface.cpp"
    "src/data-types.cpp"
    "src/friends.cpp"
    "src/intervals-manager.cpp"
    "src/messages.cpp"
    "src/requests-manager.cpp"
)

add_subdirectory("test")

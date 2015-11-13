add_library("vk-api"
    "src/communication-interface.cpp"
    "src/data-types.cpp"
    "src/exceptions.cpp"
    "src/friends.cpp"
    "src/intervals-manager.cpp"
    "src/messages.cpp"
    "src/requests-manager.cpp"
    "src/users.cpp"
)

add_subdirectory("test")

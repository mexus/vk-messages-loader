add_library("vk-api"
    "src/communication-interface.cpp"
    "src/data-types.cpp"
    "src/exceptions.cpp"
    "src/friends.cpp"
    "src/intervals-manager.cpp"
    "src/messages.cpp"
    "src/photo-attachment.cpp"
    "src/requests-manager.cpp"
    "src/sticker-attachment.cpp"
    "src/users.cpp"
    "src/video-attachment.cpp"
)

add_subdirectory("test")

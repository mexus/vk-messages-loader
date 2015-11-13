add_library("storage"
    "src/exceptions.cpp"
    "src/history-db.cpp"
    "src/history.cpp"
    "src/message.cpp"
)
add_subdirectory("test")

set(PROJECT_SOURCES
        src/main.cpp
        src/consumer.h
        src/consumer.cpp
        ../common/socket.h
        ../common/socket.cpp
)
add_executable(sp ${PROJECT_SOURCES})

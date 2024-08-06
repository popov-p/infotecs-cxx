set(PROJECT_SOURCES
        src/main.cpp
        src/producer.h
        src/producer.cpp
        src/shared-buffer.h
        src/shared-buffer.cpp
        ../common/data-processor.h
        ../common/data-processor.cpp
        ../common/socket.h
        ../common/socket.cpp
)

add_executable(fp ${PROJECT_SOURCES})


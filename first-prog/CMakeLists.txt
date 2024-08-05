set(PROJECT_SOURCES
        src/main.cpp
        src/producer.h
        src/producer.cpp
        src/shared-buffer.h
        src/shared-buffer.cpp
        src/data-processor.h
        src/data-processor.cpp
)

add_executable(fp ${PROJECT_SOURCES})


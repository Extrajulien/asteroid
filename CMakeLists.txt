cmake_minimum_required(VERSION 3.16)

project(AsteroidGame C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(game)

add_executable(${PROJECT_NAME}
        main.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE game)
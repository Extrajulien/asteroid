cmake_minimum_required(VERSION 3.20)

project(AsteroidGame C CXX)

set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(game)

add_executable(${PROJECT_NAME}
        main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE game)
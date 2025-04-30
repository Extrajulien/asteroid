cmake_minimum_required(VERSION 3.16)

project(AsteroidClion C)

set(CMAKE_C_STANDARD 11)

# Dependencies
include(FetchContent)

set(RAYLIB_VERSION 4.5.0)

FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(BUILD_EXAMPLES OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(raylib)

set_target_properties(raylib PROPERTIES LINKER_LANGUAGE C)

add_executable(AsteroidClion
        main.c
        Player/player.c
        Asteroids/asteroids.c
        Asteroids/asteroids.h
        Menu/menu.c
        Menu/menu.h
)

target_include_directories(AsteroidClion PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Player
        ${CMAKE_CURRENT_SOURCE_DIR}/Asteroids
        ${CMAKE_CURRENT_SOURCE_DIR}/Menu
)

target_link_libraries(AsteroidClion PRIVATE raylib winmm gdi32 opengl32)
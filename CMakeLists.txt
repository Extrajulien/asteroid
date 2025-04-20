cmake_minimum_required(VERSION 3.16)
project(AsteroidClion C)

set(CMAKE_C_STANDARD 11)
# Raylib paths
set(RAYLIB_DIR "/raylib/src")
set(RAYLIB_LIB "${RAYLIB_DIR}/libraylib.a")
include_directories("${RAYLIB_DIR}")

add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES
        IMPORTED_LOCATION ${RAYLIB_LIB}
)

add_executable(AsteroidClion
        main.c
        Player/player.c
        Asteroids/asteroids.c
        Asteroids/asteroids.h
)

target_include_directories(AsteroidClion PRIVATE
        ${RAYLIB_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Player
        ${CMAKE_CURRENT_SOURCE_DIR}/Asteroids
)
#windows target_link_libraries(AsteroidClion PRIVATE raylib winmm gdi32 opengl32)
#linux   target_link_libraries(AsteroidClion PRIVATE raylib m)
target_link_libraries(AsteroidClion PRIVATE raylib m)
cmake_minimum_required(VERSION 3.16)

project(AsteroidClion C)

set(CMAKE_C_STANDARD 11)

# === Dependencies ===
include(FetchContent)

set(RAYLIB_VERSION 4.5.0)

# raylib
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG ${RAYLIB_VERSION}
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# raygui
FetchContent_Declare(
        raygui
        GIT_REPOSITORY https://github.com/raysan5/raygui.git
        GIT_TAG 4.0
)

# Disable raylib examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Download dependencies
FetchContent_MakeAvailable(raylib raygui)

message(STATUS "raygui source directory: ${raygui_SOURCE_DIR}")

# === Define raygui as a library (since it has no CMakeLists.txt target) ===
add_library(raygui INTERFACE)
target_include_directories(raygui INTERFACE ${raygui_SOURCE_DIR}/src)

# === Main Executable ===
add_executable(AsteroidClion
        main.c
        Player/player.c
        Asteroids/asteroids.c
        Menu/menu.c
)

# === Include Directories ===
target_include_directories(AsteroidClion PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Player
        ${CMAKE_CURRENT_SOURCE_DIR}/Asteroids
        ${CMAKE_CURRENT_SOURCE_DIR}/Menu
        ${raygui_SOURCE_DIR}/src  # Ensure raygui header directory is included
)

# === Link Dependencies ===
target_link_libraries(AsteroidClion PRIVATE raylib raygui)

# === Platform-specific system libraries ===
if (WIN32)
    target_link_libraries(AsteroidClion PRIVATE winmm gdi32 opengl32)
elseif(UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(AsteroidClion PRIVATE m pthread dl GL X11)
endif()
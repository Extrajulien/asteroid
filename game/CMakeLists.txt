add_library(game STATIC
        game.c
)

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

# === Define raygui as a library (since it has no CMakeLists.txt target) ===
add_library(raygui INTERFACE)
target_include_directories(raygui INTERFACE ${raygui_SOURCE_DIR}/src)
add_library(style_jungle INTERFACE)
target_include_directories(style_jungle INTERFACE ${raygui_SOURCE_DIR}/styles/jungle)



target_include_directories(game PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${raygui_SOURCE_DIR}/src  # Ensure raygui header directory is included
)


add_subdirectory(preset_manager)
add_subdirectory(menu)
add_subdirectory(core)


target_link_libraries(game PUBLIC raygui raylib presetManager menu core)


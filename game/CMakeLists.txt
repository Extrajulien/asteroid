add_library(game STATIC
        game.c
        include/game_api.h
)

# === Dependencies ===
include(FetchContent)

set(RAYLIB_VERSION 5.5)

# raylib
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG ${RAYLIB_VERSION}
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Disable raylib examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Download dependencies
FetchContent_MakeAvailable(raylib)

target_include_directories(game PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

add_subdirectory(raygui_implementation)
add_subdirectory(preset_manager)
add_subdirectory(menu)
add_subdirectory(core)
add_subdirectory(event)
add_subdirectory(event/event_sink)
add_subdirectory(event/event_api)
add_subdirectory(game_loop)


target_link_libraries(game
        PRIVATE raylib
        PUBLIC raygui_implementation
        PRIVATE presetManager
        PUBLIC menu
        PUBLIC asteroid
        PRIVATE core
        PRIVATE event
        PUBLIC game_loop
        INTERFACE event_api
        PRIVATE event_sink_api
)


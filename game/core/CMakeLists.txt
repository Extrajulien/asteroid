add_library(core STATIC
        src/player.c
        src/particle.c
        src/bullet_array.c
        include/game_math.h
        include/components.h
        include/entity_info.h
)
add_subdirectory(asteroid)

target_include_directories(core PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(core PRIVATE game PUBLIC asteroid PRIVATE event_sink_api PUBLIC raylib)
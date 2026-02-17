add_library(game_loop STATIC
        src/game_loop.c
        src/wave.c
        src/asteroid_explosion_rule.c
        src/asteroid_wave_spawn_rule.c
        src/player_rules.c
)

target_include_directories(game_loop PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(game_loop PRIVATE raylib core event asteroid logger PUBLIC game)
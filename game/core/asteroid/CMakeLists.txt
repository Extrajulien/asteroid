add_library(asteroid STATIC
        src/asteroid.c
        src/wave.c
        src/asteroid_array.c
        src/asteroid_event.c
        src/asteroid_preset.c
        include/asteroid_size.h
        src/asteroid_wave_spawn_rule.c
        src/asteroid_explosion_rule.c
)

target_include_directories(asteroid PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(asteroid PUBLIC core raylib)
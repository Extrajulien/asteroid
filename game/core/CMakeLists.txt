add_library(core STATIC
        Asteroid/src/asteroid.c
        src/player.c
        src/particle.c
        include/game_math.h
        include/components.h
        include/entity_info.h
)
add_subdirectory(asteroid)

target_include_directories(core PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(core PUBLIC game asteroid)
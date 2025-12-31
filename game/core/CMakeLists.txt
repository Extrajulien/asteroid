add_library(core STATIC
        src/asteroid.c
        src/player.c
        src/particle.c
)

target_include_directories(core PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(core PUBLIC game)
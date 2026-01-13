add_library(asteroid STATIC
        src/asteroid.c
        src/asteroid_array.c
        src/asteroid_preset.c
        include/asteroid_size.h
)

target_include_directories(asteroid PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(asteroid PUBLIC core PRIVATE raylib INTERFACE event_sink_api)
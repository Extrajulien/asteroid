add_library(event STATIC
        src/asteroid_bullet_hit_event_queue.c
        src/player_asteroid_hit_event_queue.c
)

target_include_directories(event PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
)

include_directories(event_sink)
include_directories(event_api)

target_link_libraries(event
        PUBLIC core
        PRIVATE game
        PUBLIC asteroid
        INTERFACE event_api
        PRIVATE event_sink_api
        PUBLIC raylib
        PRIVATE logger
)
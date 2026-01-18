add_library(event_sink_api
        STATIC event_sink_api.h event_sink.c
)

target_include_directories(event_sink_api PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../include
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(event_sink_api PRIVATE raylib PRIVATE event)
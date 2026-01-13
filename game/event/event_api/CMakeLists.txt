add_library(event_api
        INTERFACE event_api.h
)

target_include_directories(event_api INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../include
        ${CMAKE_CURRENT_LIST_DIR}
)
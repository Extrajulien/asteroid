add_library(logger STATIC
        src/logger.c
)


target_include_directories(logger
        PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
        PUBLIC include/api
)

target_link_libraries(logger PRIVATE utils)
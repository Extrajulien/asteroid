add_library(utils STATIC
        src/char_array.c
)


target_include_directories(utils PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(utils PRIVATE logger)
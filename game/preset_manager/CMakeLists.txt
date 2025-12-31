add_library(presetManager STATIC
        src/files.c
)


target_include_directories(presetManager PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(presetManager PUBLIC game)
add_library(menu STATIC
        src/menu.c
)


target_include_directories(menu PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(menu PUBLIC raylib raygui style_jungle core)
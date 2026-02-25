add_library(menu STATIC
        src/menu.c
        src/screen.c
        src/screen_title.c
        src/screen_preset_create.c
        src/screen_game.c
        src/screen_game_over.c
        include/screen_virtual_table.h
        include/screen_context.h
        include/menu_style.h
)


target_include_directories(menu PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)


target_link_libraries(menu
        PRIVATE raylib
        PRIVATE raygui_implementation
        PRIVATE presetManager
        PRIVATE core
        PRIVATE game
)
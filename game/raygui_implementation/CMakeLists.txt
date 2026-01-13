FetchContent_Declare(
        raygui
        GIT_REPOSITORY https://github.com/raysan5/raygui.git
        GIT_TAG master
)
FetchContent_MakeAvailable(raygui)

add_library(raygui INTERFACE)
target_include_directories(raygui INTERFACE ${raygui_SOURCE_DIR}/src)
add_library(style_amber INTERFACE)
target_include_directories(style_amber INTERFACE ${raygui_SOURCE_DIR}/styles/amber)

add_library(raygui_implementation
        STATIC raygui_implementation.c
)
target_include_directories(raygui_implementation
        PRIVATE ${raygui_SOURCE_DIR}/src
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(raygui_implementation
        PRIVATE raylib
        PUBLIC raygui
        PRIVATE style_amber
)
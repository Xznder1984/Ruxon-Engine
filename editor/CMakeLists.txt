add_executable(ruxon_editor
    main.cpp
    ui/theme.cpp
    docks/scene_tree_dock.cpp
    docks/inspector_dock.cpp
    docks/viewport_dock.cpp
    about.cpp
    # Vendored Dear ImGui (docking) + SDL3/OpenGL3 backends
    ../thirdparty/imgui/imgui.cpp
    ../thirdparty/imgui/imgui_draw.cpp
    ../thirdparty/imgui/imgui_tables.cpp
    ../thirdparty/imgui/imgui_widgets.cpp
    ../thirdparty/imgui/backends/imgui_impl_sdl3.cpp
    ../thirdparty/imgui/backends/imgui_impl_opengl3.cpp
)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(ruxon_editor PRIVATE ruxon_engine SDL3::SDL3)

target_include_directories(ruxon_editor PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/editor
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui
    ${CMAKE_SOURCE_DIR}/thirdparty/imgui/backends
)

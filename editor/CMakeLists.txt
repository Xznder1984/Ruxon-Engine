add_executable(ruxon_editor
    main.cpp
    ui/theme.cpp
    docks/scene_tree_dock.cpp
    docks/inspector_dock.cpp
    docks/viewport_dock.cpp
    about.cpp
)

target_link_libraries(ruxon_editor PRIVATE ruxon_engine)

target_include_directories(ruxon_editor PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/editor
)

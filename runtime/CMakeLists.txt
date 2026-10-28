add_executable(ruxon_player
    main.cpp
)

target_link_libraries(ruxon_player PRIVATE ruxon_engine)
target_include_directories(ruxon_player PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(ruxon_player PRIVATE ruxon_engine)

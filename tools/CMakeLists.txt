add_executable(ruxon
    main.cpp
)

target_link_libraries(ruxon PRIVATE ruxon_engine)
target_include_directories(ruxon PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(ruxon PRIVATE ruxon_engine)
add_subdirectory(lsp)

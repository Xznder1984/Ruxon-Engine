add_executable(ruxon-lsp
    main.cpp
)

target_link_libraries(ruxon-lsp PRIVATE ruxon_engine)
target_include_directories(ruxon-lsp PRIVATE ${CMAKE_SOURCE_DIR}/include)

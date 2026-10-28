add_executable(ruxon_tests
    main.cpp
)

target_link_libraries(ruxon_tests PRIVATE ruxon_engine)
target_include_directories(ruxon_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(NAME basic_test COMMAND ruxon_tests)
target_link_libraries(ruxon_tests PRIVATE ruxon_engine)

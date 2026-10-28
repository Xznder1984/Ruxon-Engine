add_library(ruxon_engine
    core/log.cpp
    core/application.cpp
    platform/window.cpp
    scene/node.cpp
    render/rhi.cpp
    render/opengl/opengl_buffer.cpp
    scripting/script.cpp
    scripting/lexer/lexer.cpp
    scripting/vm/vm.cpp
    ai/assistant.cpp
)

target_include_directories(ruxon_engine PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(ruxon_engine PUBLIC glm)

target_compile_features(ruxon_engine PUBLIC cxx_std_20)

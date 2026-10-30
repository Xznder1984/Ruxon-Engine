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

target_compile_features(ruxon_engine PUBLIC cxx_std_20)

find_package(OpenGL REQUIRED)
target_link_libraries(ruxon_engine PUBLIC glm OpenGL::GL)

find_package(SDL3 CONFIG QUIET)
if(SDL3_FOUND)
    target_link_libraries(ruxon_engine PRIVATE SDL3::SDL3)
    target_compile_definitions(ruxon_engine PRIVATE RUXON_HAVE_SDL3)
else()
    message(FATAL_ERROR "SDL3 not found. Install it (macOS: brew install sdl3, Ubuntu: sudo apt install libsdl3-dev) and re-run CMake.")
endif()

cmake_minimum_required(VERSION 3.20)
project(RuxonEngine VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options
option(RUXON_BUILD_EDITOR "Build the editor" ON)
option(RUXON_BUILD_RUNTIME "Build the runtime player" ON)
option(RUXON_MODULE_3D "Enable 3D module" ON)
option(RUXON_MODULE_PHYSICS2D "Enable 2D physics" ON)
option(RUXON_MODULE_PHYSICS3D "Enable 3D physics" ON)
option(RUXON_MODULE_AUDIO "Enable audio" ON)
option(RUXON_MODULE_SCRIPTING "Enable scripting" ON)
option(RUXON_MODULE_AI "Enable AI assistant (BYOK)" ON)

# Add subdirectories
add_subdirectory(thirdparty)
add_subdirectory(src)
if(RUXON_BUILD_EDITOR)
    add_subdirectory(editor)
endif()
if(RUXON_BUILD_RUNTIME)
    add_subdirectory(runtime)
endif()
add_subdirectory(tools)

enable_testing()
add_subdirectory(tests)

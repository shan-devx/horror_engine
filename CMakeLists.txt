cmake_minimum_required(VERSION 3.14)
project(horror_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master 
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)
FetchContent_Declare(
    rlimgui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG main
)
FetchContent_MakeAvailable(rlimgui)

set(RLIMGUI_SOURCES
  ${rlimgui_SOURCE_DIR}/rlImGui.cpp

  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

add_executable(horror_engine 
  ${RLIMGUI_SOURCES}
  src/engine.cpp
  src/main.cpp
  src/scripts/player.cpp
)

target_include_directories(horror_engine PRIVATE
    ${raylib_SOURCE_DIR}/src
    ${imgui_SOURCE_DIR}
    ${rlimgui_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(horror_engine PRIVATE
  raylib
)

target_compile_definitions(horror_engine PRIVATE 
  IMGUI_DISABLE_OBSOLETE_FUNCTIONS 
  IMGUI_DISABLE_OBSOLETE_KEYIO
)

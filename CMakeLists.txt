cmake_minimum_required(VERSION 3.14)
project(horror_engine)

set(CMAKE_CXX_STANDARD 20)

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
    rlImGui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG main
)
FetchContent_MakeAvailable(rlImGui)

add_executable(horror_engine 
  # this should work
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${rlimgui_SOURCE_DIR}/rlImGui.cpp

  src/gui.cpp
)

target_include_directories(horror_engine 
  PRIVATE ${imgui_SOURCE_DIR}
  PRIVATE ${rlImGui_SOURCE_DIR}
)
target_link_libraries(horror_engine
  PRIVATE raylib
)

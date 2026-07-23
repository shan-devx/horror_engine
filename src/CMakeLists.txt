cmake_minimum_required(VERSION 3.14)
project(horror_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 6.0
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json)

add_executable(game
  persistence.cpp
  scripts/player.cpp
  runtime.cpp
)

target_include_directories(game PRIVATE
    ${raylib_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(game PRIVATE
  raylib
  nlohmann_json::nlohmann_json
)

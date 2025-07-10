cmake_minimum_required(VERSION 3.16)

project(Obsidian-Canvas-Parser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Obsidian-Canvas-Parser Obs_canvas_parser_v1.0.cpp)

include(GNUInstallDirs)
install(TARGETS Obsidian-Canvas-Parser
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0  # or your preferred version
)

FetchContent_MakeAvailable(json)

target_link_libraries(Obsidian-Canvas-Parser
    PRIVATE
      nlohmann_json::nlohmann_json
)


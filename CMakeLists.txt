cmake_minimum_required(VERSION 3.16)
project(ObsidianCanvasParser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE HEADER_FILES
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
)

add_executable(ObsidianCanvasParser-v1.0
    Obs_canvas_parser_v1.0.cpp
)

target_include_directories(ObsidianCanvasParser-v1.0 PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

if (MINGW)
    set_target_properties(ObsidianCanvasParser-v1.0 PROPERTIES WIN32_EXECUTABLE OFF)
    target_link_options(ObsidianCanvasParser-v1.0 PRIVATE
        "-static"
        "-static-libgcc"
        "-static-libstdc++"
    )
endif()

# Make headers visible in Qt Creator
add_custom_target(headers SOURCES ${HEADER_FILES})

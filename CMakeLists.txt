cmake_minimum_required(VERSION 3.16)
project(LatencyX VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT WIN32)
    message(FATAL_ERROR "LatencyX esta disenado exclusivamente para plataformas Windows.")
endif()

target_include_directories(LatencyX PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/imgui-master"
    "${CMAKE_SOURCE_DIR}/include/imgui-master/backends"
)

file(GLOB SOURCES "src/*.cpp")

add_executable(LatencyX ${SOURCES})

target_link_libraries(LatencyX PRIVATE
    winmm
    d3d11
    dxgi
    user32
    gdi32
)

if(MSVC)
    target_compile_options(LatencyX PRIVATE /W4 /O2)
else()
    target_compile_options(LatencyX PRIVATE -Wall -Wextra -O2)
endif()
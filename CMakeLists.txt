cmake_minimum_required(VERSION 3.23)
project(TouhouGame C)

set(CMAKE_C_STANDARD 99)

# External dependencies
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

# SDL2 Loading
if (WIN32) # Windows uses local dependencies
    set(SDL2_PATH "externals/SDL2-2.24.0/x86_64-w64-mingw32")
endif ()
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(TouhouGame main.c)
target_link_libraries(TouhouGame ${SDL2_LIBRARY})

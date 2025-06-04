cmake_minimum_required(VERSION 3.14)
project(literaster)

add_subdirectory(external/yaml-cpp)

# C++ version and warnings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

# Output binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Include directories
include_directories(
    include
)

# Source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# Add executable
add_executable(literaster ${SRC_FILES})

# Link raylib and required system libraries (Linux-specific)
target_link_libraries(literaster
    raylib
    m
    dl
    pthread
    GL
    rt
    X11
    yaml-cpp
)

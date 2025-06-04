cmake_minimum_required(VERSION 3.14)
project(literaster)

add_subdirectory(external/yaml-cpp)
add_subdirectory(external/raylib)

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
    yaml-cpp
)

# On Linux, raylib may need these extra system libs
if(UNIX AND NOT APPLE)
    target_link_libraries(literaster m dl pthread GL X11 rt)
endif()

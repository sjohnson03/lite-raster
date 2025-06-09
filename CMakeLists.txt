cmake_minimum_required(VERSION 3.14)
project(literaster)

include(FetchContent)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
)

# Configure yaml-cpp to not build tests/tools
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(yaml-cpp raylib)

# C++ version and warnings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/std:c++17)
endif()

if(MSVC)
    # MSVC warnings
    add_compile_options(/W4)
else()
    # GCC/Clang warnings
    add_compile_options(-Wall -Wextra)
endif()

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

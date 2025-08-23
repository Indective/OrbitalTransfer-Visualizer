cmake_minimum_required(VERSION 3.16)
project(simulation)

set(CMAKE_CXX_STANDARD 17)

# Add include and src directories
include_directories(include)

# Recursively find all source files in src/
file(GLOB_RECURSE SOURCES src/*.cpp)

# Find Raylib
find_package(raylib REQUIRED)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link Raylib
target_link_libraries(${PROJECT_NAME} raylib)

# On Linux, link required system libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m pthread dl rt X11)
endif()

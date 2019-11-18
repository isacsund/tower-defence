# CMake >= 3.0 is required
cmake_minimum_required(VERSION 3.0)

# Set the project name
project(tower-defence)

# Define the output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

# Disable in-source builds
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Set macro directory
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/macros")

# Include the macros to use
include(auto_collect)

# Find the SFML libraries
find_package(SFML 2.4 REQUIRED audio graphics window system)

find_package(Boost REQUIRED filesystem)

# Add TGUI sources
add_subdirectory(deps/TGUI-0.7.8)

# Add core sources
add_subdirectory(src)
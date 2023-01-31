cmake_minimum_required(VERSION 3.8)
project("proyekgen" VERSION 0.4.3 LANGUAGES CXX)

# Modify CMake variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define project variables
set(PROJECT_ROOT_PATH ${CMAKE_SOURCE_DIR})

# Add "proyekgen" subproject to compile executable
add_subdirectory("${PROJECT_ROOT_PATH}/proyekgen")
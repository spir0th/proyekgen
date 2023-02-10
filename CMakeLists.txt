cmake_minimum_required(VERSION 3.21)
project("proyekgen" VERSION 1.0.0 LANGUAGES CXX)

# Modify CMake variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define project variables
set(PROJECT_ROOT_PATH ${PROJECT_SOURCE_DIR})

# Add "proyekgen" subproject
add_subdirectory("${PROJECT_ROOT_PATH}/proyekgen")
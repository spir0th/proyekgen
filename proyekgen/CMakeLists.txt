cmake_minimum_required(VERSION 3.0)
set(PROYEKGEN_TARGET_NAME "proyekgen")
set(PROYEKGEN_HEADERS "template.h" "generator.h")
set(PROYEKGEN_SOURCES "main.cpp" "template.cpp" "generator.cpp")

find_path(ZSTR_INCLUDE_DIRS "zstr.hpp")

add_executable(${PROYEKGEN_TARGET_NAME} ${PROYEKGEN_HEADERS} ${PROYEKGEN_SOURCES})
target_include_directories(${PROYEKGEN_TARGET_NAME} PRIVATE ${ZSTR_INCLUDE_DIRS})
install(TARGETS ${PROYEKGEN_TARGET_NAME} RUNTIME DESTINATION bin)
include(CPack)
cmake_minimum_required(VERSION 3.17)
set(PROYEKGEN_TARGET_NAME "proyekgen")
set(PROYEKGEN_HEADERS "template.h"  "config.h" "console.h" "system.h" "error.h" "global.h" "cmdline.h")
set(PROYEKGEN_SOURCES "main.cpp" "template.cpp"  "config.cpp" "console.cpp" "system.cpp" "error.cpp" "cmdline.cpp")

find_package(nlohmann_json CONFIG REQUIRED)
find_package(LibArchive REQUIRED)

add_executable(${PROYEKGEN_TARGET_NAME} ${PROYEKGEN_HEADERS} ${PROYEKGEN_SOURCES})
target_include_directories(${PROYEKGEN_TARGET_NAME} PRIVATE ${ZSTR_INCLUDE_DIRS})
target_link_libraries(${PROYEKGEN_TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json LibArchive::LibArchive)
install(TARGETS ${PROYEKGEN_TARGET_NAME} RUNTIME DESTINATION bin)
include(CPack)
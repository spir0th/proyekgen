cmake_minimum_required(VERSION 3.0)
set(PROYEKGEN_TARGET_NAME "proyekgen")
set(PROYEKGEN_HEADERS "template.h" "generator.h" "config.h" "console.h")
set(PROYEKGEN_SOURCES "main.cpp" "template.cpp" "generator.cpp" "config.cpp" "console.cpp")

find_package(nlohmann_json CONFIG REQUIRED)

add_executable(${PROYEKGEN_TARGET_NAME} ${PROYEKGEN_HEADERS} ${PROYEKGEN_SOURCES})
target_include_directories(${PROYEKGEN_TARGET_NAME} PRIVATE ${ZSTR_INCLUDE_DIRS})
target_link_libraries(${PROYEKGEN_TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json)
install(TARGETS ${PROYEKGEN_TARGET_NAME} RUNTIME DESTINATION bin)
include(CPack)
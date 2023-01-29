cmake_minimum_required(VERSION 3.17)
set(PROYEKGEN_TARGET_NAME "proyekgen")
set(PROYEKGEN_HEADERS "template.h"  "config.h" "console.h" "system.h" "error.h" "global.h" )
set(PROYEKGEN_SOURCES "main.cpp" "template.cpp"  "config.cpp" "console.cpp" "system.cpp" "error.cpp" )

find_package(cxxopts CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(LibArchive REQUIRED)

add_executable(${PROYEKGEN_TARGET_NAME} ${PROYEKGEN_HEADERS} ${PROYEKGEN_SOURCES})
target_link_libraries(${PROYEKGEN_TARGET_NAME} PRIVATE
	cxxopts::cxxopts nlohmann_json::nlohmann_json
	LibArchive::LibArchive
)
cmake_minimum_required(VERSION 3.17)
set(PROYEKGEN_TARGET_NAME "proyekgen")
set(PROYEKGEN_HEADERS "template.h"   "console.h" "system.h" "error.h" "global.h" )
set(PROYEKGEN_SOURCES "main.cpp" "template.cpp"   "console.cpp" "system.cpp" "error.cpp" )

find_package(cxxopts CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(LibArchive REQUIRED)

if(WIN32)
find_path(LIBCONFIG_INCLUDE_DIRS libconfig.h)
find_library(LIBCONFIG_LIBRARIES libconfig)
find_path(LIBCONFIG++_INCLUDE_DIRS libconfig.h)
find_library(LIBCONFIG++_LIBRARIES libconfig++)
elseif(UNIX)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig)
pkg_check_modules(LIBCONFIG++ REQUIRED libconfig++)
endif()

add_executable(${PROYEKGEN_TARGET_NAME} ${PROYEKGEN_HEADERS} ${PROYEKGEN_SOURCES})
target_include_directories(${PROYEKGEN_TARGET_NAME} PRIVATE
	${LIBCONFIG_INCLUDE_DIRS} ${LIBCONFIG++_INCLUDE_DIRS}
)
target_link_libraries(${PROYEKGEN_TARGET_NAME} PRIVATE
	cxxopts::cxxopts nlohmann_json::nlohmann_json
	LibArchive::LibArchive ${LIBCONFIG_LIBRARIES}
	${LIBCONFIG++_LIBRARIES}
)
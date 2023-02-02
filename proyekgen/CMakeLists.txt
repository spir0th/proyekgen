# Define target variables
set(PROYEKGEN_TARGET_NAME "proyekgen")
set(PROYEKGEN_HEADERS "template.h" "system.h" "global.h")
set(PROYEKGEN_SOURCES "main.cpp" "template.cpp" "system.cpp")

# Find required libraries
find_package(cxxopts CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(LibArchive REQUIRED)

# libconfig might not be available using find_package
if(WIN32)
find_path(LIBCONFIG++_INCLUDE_DIRS libconfig.h++)
find_library(LIBCONFIG++_LIBRARIES libconfig++)
elseif(UNIX)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG++ REQUIRED libconfig++)
endif()

# Generate executable and link libraries
add_executable(${PROYEKGEN_TARGET_NAME} ${PROYEKGEN_HEADERS} ${PROYEKGEN_SOURCES})
target_include_directories(${PROYEKGEN_TARGET_NAME} PRIVATE ${LIBCONFIG++_INCLUDE_DIRS})
target_link_libraries(${PROYEKGEN_TARGET_NAME} PRIVATE
	cxxopts::cxxopts fmt::fmt nlohmann_json::nlohmann_json
	LibArchive::LibArchive ${LIBCONFIG++_LIBRARIES}
)
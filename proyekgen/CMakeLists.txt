# Define target variables
set(PROYEKGEN_TARGET_NAME "proyekgen")
set(PROYEKGEN_TARGET_VERSION ${PROJECT_VERSION})
set(PROYEKGEN_HEADERS "template.h" "system.h" "global.h" "input.h")
set(PROYEKGEN_SOURCES "main.cpp" "template.cpp" "system.cpp" "input.cpp")

# Find required libraries
find_package(cxxopts CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(LibArchive REQUIRED)

# Find Lua by using find_package in Windows and find_path in UNIX
if(WIN32)
find_package(Lua REQUIRED)
elseif(UNIX)
find_path(LUA_INCLUDE_DIRS lua.hpp)
find_library(LUA_LIBRARIES lua)
endif()

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
target_compile_definitions(${PROYEKGEN_TARGET_NAME} PUBLIC
	PROYEKGEN_HELP_NAME="${PROYEKGEN_TARGET_NAME}"
	PROYEKGEN_HELP_VERSION="${PROYEKGEN_TARGET_VERSION}"
)
target_include_directories(${PROYEKGEN_TARGET_NAME} PRIVATE
	${LIBCONFIG++_INCLUDE_DIRS}
	${LUA_INCLUDE_DIR}
)
target_link_libraries(${PROYEKGEN_TARGET_NAME} PRIVATE
	cxxopts::cxxopts fmt::fmt nlohmann_json::nlohmann_json
	LibArchive::LibArchive ${LIBCONFIG++_LIBRARIES} ${LUA_LIBRARIES}
)
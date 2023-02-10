# Find required libraries
find_package(cxxopts CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(LibArchive REQUIRED)

if(WIN32)
	find_package(Lua REQUIRED)
	find_path(LIBCONFIG++_INCLUDE_DIRS libconfig.h++)
	find_library(LIBCONFIG++_LIBRARIES libconfig++)
elseif(UNIX)
	find_package(PkgConfig REQUIRED)
	pkg_check_modules(LIBCONFIG++ REQUIRED libconfig++)
	pkg_check_modules(LUA REQUIRED lua)
endif()

# Define targets variables
set(PROYEKGEN_HEADERS "template.h" "system.h" "global.h" "input.h")
set(PROYEKGEN_SOURCES "main.cpp" "template.cpp" "system.cpp" "input.cpp")

# Generate target executable
add_executable(proyekgen ${PROYEKGEN_HEADERS} ${PROYEKGEN_SOURCES})
target_compile_definitions(proyekgen PUBLIC
	PROYEKGEN_HELP_NAME="${PROJECT_NAME}"
	PROYEKGEN_HELP_VERSION="${PROJECT_VERSION}"
)
target_include_directories(proyekgen PRIVATE
	${LIBCONFIG++_INCLUDE_DIRS}
	${LUA_INCLUDE_DIR}
)
target_link_libraries(proyekgen PRIVATE
	cxxopts::cxxopts fmt::fmt nlohmann_json::nlohmann_json
	LibArchive::LibArchive ${LIBCONFIG++_LIBRARIES} ${LUA_LIBRARIES}
)

# Use CPack to distribute proyekgen
set(CPACK_PACKAGE_VENDOR "spirothXYZ")
set(CPACK_PACKAGE_NAME "proyekgen")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Project Generator with extensive templates!")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "proyekgen")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(PROYEKGEN_RUNTIME_DESTINATION ".")

install(TARGETS proyekgen
	RUNTIME_DEPENDENCY_SET dependencies
	DESTINATION ${PROYEKGEN_RUNTIME_DESTINATION}
	RUNTIME
)
install(RUNTIME_DEPENDENCY_SET dependencies
	POST_EXCLUDE_REGEXES ".*Windows[\\/]system32.*|.*Windows[\\/]SysWOW64.*|^/lib" "^/usr" "^/bin"
	DESTINATION ${PROYEKGEN_RUNTIME_DESTINATION}
)

if(WIN32)
	# Include required MSVC libraries in Windows
	set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
	include(InstallRequiredSystemLibraries)

	install(PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
		DESTINATION ${PROYEKGEN_RUNTIME_DESTINATION}
	)
endif()

include(CPack)
cmake_minimum_required(VERSION 3.14)

project(cpp-json)

include(FetchContent)

FetchContent_Declare(
	cpp-types
	GIT_REPOSITORY https://github.com/TomBeacock/cpp-types.git
	GIT_TAG origin/main
)

FetchContent_Declare(
	cpp-parser
	GIT_REPOSITORY https://github.com/TomBeacock/cpp-parser.git
	GIT_TAG origin/main
)

FetchContent_MakeAvailable(cpp-types cpp-parser)

add_library(cpp-json STATIC
	"include/json/json_types.h"	"src/json_types.cpp"
	"include/json/json_parser.h" "src/json_parser.cpp"
)

set_target_properties(cpp-json PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON)
target_include_directories(cpp-json
	PUBLIC
		"${PROJECT_SOURCE_DIR}/include"
	PRIVATE
		"${PROJECT_SOURCE_DIR}/src"
)
target_link_libraries(cpp-json PUBLIC cpp-types cpp-parser)

# Standalone options
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	option(JSON_BUILD_TESTS "Build test suite" ON)

	if(JSON_BUILD_TESTS)
		add_subdirectory(tests)
	endif()
endif()
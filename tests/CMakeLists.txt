cmake_minimum_required(VERSION 3.14)

message(STATUS "Buliding tests")

include(FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG b514bdc898e2951020cbdca1304b75f5950d1f59
)

if(WIN32)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
	"json_parser.cpp"
)
target_link_libraries(tests PRIVATE cpp-json GTest::gtest_main)
set_target_properties(tests PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON)

include(GoogleTest)
gtest_discover_tests(tests)
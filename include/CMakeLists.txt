cmake_minimum_required(VERSION 3.8)

project(FinalProject)

include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-1.12.1
)
FetchContent_MakeAvaible(googletest)

enable_testing()

set(TEST_SOURCES
		InvertedIndex_test.cpp
		SearchEngine_test.cpp
		ConverterJSON_test.cpp

		../src/ConverterJSON.cpp
		../src/SearchEngine.cpp
		../src/InvertedIndex.cpp
)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
		${gtest_SOURCE_DIR}/include ${INCLUDE_DIR}		
		${MY_INCLUDE_DIR}
)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(YourProject PRIVATE nlohmann_json::nlohmann_json)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.14.0
)

FetchContent_MakeAvailable(googletest)

find_package(GTest REQUIRED)
target_link_libraries(YourProject PRIVATE GTest::GTest GTest::Main)
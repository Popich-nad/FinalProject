cmake_minimum_required(VERSION 3.8)

project(SearchEnigneTests)

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

target_link_libraries(${PROJECT_NAME} PRIVATE
		gtest_main
		nlohmann_json::nlohmann_json
)
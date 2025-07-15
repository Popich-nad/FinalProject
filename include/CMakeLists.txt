cmake_minimum_required(VERSION 3.10)

set(CMAKE_GENERATOR "MinGW Makefiles" CACHE INTERNAL "")

set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
set(CMAKE_MAKE_PROGRAM "C:/msys64/mingw64/bin/mingw32-make.exe")

project(FinalProject LANGUAGES CXX)

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(MY_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/../src")

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.14.0
)
FetchContent_MakeAvailable(googletest)

find_package(GTest REQUIRED)
target_include_directories(FinalProject PUBLIC ${GTEST_INCLUDE_DIRS})

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

enable_testing()

set(TEST_SOURCES
    InvertedIndex_test.cpp
    SearchEngine_test.cpp
    ConverterJSON_test.cpp
    
    ../src/ConverterJSON.cpp
    ../src/SearchEngine.cpp
    ../src/InvertedIndex.cpp
)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

target_include_directories(${PROJECT_NAME}_tests PUBLIC
    ${gtest_SOURCE_DIR}/include
    ${INCLUDE_DIR}        
    ${MY_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE 
    GTest::GTest 
    GTest::Main
    nlohmann_json::nlohmann_json
)
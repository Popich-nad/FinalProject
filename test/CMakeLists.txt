cmake_minimum_required(VERSION 3.8)
project(*FinalProject*)

set(CMAKE_CXX_STANDARD 20)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(tests)

add_executable(test_gtest test_gtest.cpp)
target_link_libraries(test_gtest PRIVATE gtest gtest_main pthread)
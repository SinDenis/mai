cmake_minimum_required(VERSION 3.10.3)
project(oop_exercise_01)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
add_subdirectory(lib/googletest)
include_directories(lib/googletest/googletest/include)

add_executable(oop_exercise_01 main.cpp src/big_int.cpp)

add_executable(oop_exercise_01_tests test/test.cpp src/big_int.cpp)
target_link_libraries(oop_exercise_01_tests gtest gtest_main)

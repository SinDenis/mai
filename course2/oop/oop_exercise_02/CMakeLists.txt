cmake_minimum_required(VERSION 3.14)
project(oop_exercise_02)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/googletest)
add_executable(oop_exercise_02 main.cpp src/complex.cpp)

add_executable(oop_exercise_02_tests test/test.cpp src/complex.cpp )
target_link_libraries(oop_exercise_02_tests gtest gtest_main)

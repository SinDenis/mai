cmake_minimum_required(VERSION 3.14)
project(oop_exercise_03)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib/googletest)
add_executable(oop_exercise_03 main.cpp src/figure.cpp src/triangle.cpp src/point.cpp src/rectangle.cpp src/square.cpp)

add_executable(oop_exercise_03_tests test/test.cpp src/point.cpp src/triangle.cpp src/square.cpp src/rectangle.cpp)
target_link_libraries(oop_exercise_03_tests gtest gtest_main)

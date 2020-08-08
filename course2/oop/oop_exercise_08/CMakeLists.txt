cmake_minimum_required(VERSION 3.14)
project(oop_exercise_08)

find_package(Threads)

add_executable(app main.cpp)
set_target_properties(app PROPERTIES CXX_STANDARD 14)
target_link_libraries(app ${CMAKE_THREAD_LIBS_INIT})

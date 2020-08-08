cmake_minimum_required(VERSION 3.6)
project(OSLab06)

set(CMAKE_CXX_STANDARD 11)
find_package(PkgConfig)
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
		)
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
		)


add_executable(calc_node calc_node.cpp)
target_include_directories(calc_node PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(calc_node PUBLIC ${ZeroMQ_LIBRARY})

add_executable(app main.cpp)
target_include_directories(app PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(app PUBLIC ${ZeroMQ_LIBRARY})

cmake_minimum_required(VERSION 3.6)
project(KVStorage)

set(CMAKE_CXX_STANDARD 14)
find_package(PkgConfig)
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

add_executable(store1 store1.cpp)
target_include_directories(store1 PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(store1 PUBLIC ${ZeroMQ_LIBRARY})

add_executable(store2 store2.cpp)
target_include_directories(store2 PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(store2 PUBLIC ${ZeroMQ_LIBRARY})

add_executable(app app.cpp)
target_include_directories(app PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(app PUBLIC ${ZeroMQ_LIBRARY})
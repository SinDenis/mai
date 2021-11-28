cmake_minimum_required(VERSION 3.14.3)

project(AudioSearch)


add_executable(DA
        src/main.cpp
        src/Mp3/Mp3.cpp
        src/Fft/Fft.cpp
        src/ArgParsing/ArgParse.cpp
        src/Controller/Controller.cpp
        src/AudioDb/AudioDB.cpp)

find_library(MPG_LIB mpg123)
find_path(MPG_INCLUDE_DIR NAMES mpg123.h)
target_link_libraries(DA PUBLIC ${MPG_LIB})
target_include_directories(DA PUBLIC ${MPG_INCLUDE_DIR})
set(CMAKE_CXX_FLAGS "-std=c++14 -O3 -Wextra -Wall -pedantic")

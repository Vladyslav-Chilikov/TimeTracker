cmake_minimum_required(VERSION 3.22)

# set the project name
project(TimeTracker VERSION 1.0)

add_executable(TimeTracker main.cpp)

add_library(Logging Logging.h Logging.cpp)

add_dependencies(TimeTracker  Logging)

target_link_libraries(TimeTracker Logging)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


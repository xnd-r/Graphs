# Name of the project
project(Graphs)

# Minimum version of CMake required to build this project
cmake_minimum_required(VERSION 3.0)


include_directories("/home/xndr/devel/repos/rapidjson/include/rapidjson")
include_directories("/home/xndr/Downloads/boost_1_71_0/boost/heap")
include_directories("./include")
set(CMAKE_CXX_STANDARD 11)
# Add all the source files needed to build the executable
add_executable(main main.cpp)

# Name of the project
project(Graphs)

# Minimum version of CMake required to build this project
cmake_minimum_required(VERSION 3.0)


include_directories("/home/xndr/devel/repos/rapidjson/include/rapidjson")

# Add all the source files needed to build the executable
add_executable(main main.cpp)

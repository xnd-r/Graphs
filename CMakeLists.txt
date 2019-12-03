# Name of the project
project(Graphs)

# Minimum version of CMake required to build this project
cmake_minimum_required(VERSION 3.0)


include_directories("./3rdparty/rapidjson/include/rapidjson")
set(CMAKE_CXX_STANDARD 11)
# Add all the source files needed to build the executable
add_executable(main main.cpp)

cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(tabulate VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add include directory to this project
add_subdirectory(src)
add_subdirectory(runtime)
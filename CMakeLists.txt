cmake_minimum_required(VERSION 3.21)
project(ct)

find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 20)

include_directories(include ${Boost_INCLUDE_DIRS})
add_subdirectory(example)
add_subdirectory(test)

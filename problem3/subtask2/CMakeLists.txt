cmake_minimum_required(VERSION 3.10)

project(Mall)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

include_directories(src)

add_subdirectory(googletest)
add_subdirectory(src)
add_subdirectory(test)
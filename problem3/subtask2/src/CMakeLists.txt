cmake_minimum_required(VERSION 3.10)

project(Mall)

set(SOURCES mall.cpp mall.h)

add_library(mall_lib STATIC ${SOURCES})

add_executable(mall main.cpp)

target_link_libraries(mall mall_lib pthread)
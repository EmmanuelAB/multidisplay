cmake_minimum_required(VERSION 3.13)
project(multidisplay C)

set(CMAKE_C_STANDARD 99)

add_executable(multidisplay Language_Parse.h Language_Parse.c main.c)

target_link_libraries(multidisplay json-c)


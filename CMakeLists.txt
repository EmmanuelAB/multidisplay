cmake_minimum_required(VERSION 3.13)
project(multidisplay C)

set(CMAKE_C_STANDARD 99)

add_executable(multidisplay main.c scheduling.h simple_list.h test_animation.h)

target_link_libraries(multidisplay ncurses m)


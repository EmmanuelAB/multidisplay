cmake_minimum_required(VERSION 3.13)
project(multidisplay C)

set(CMAKE_C_STANDARD 99)

add_executable(multidisplay main.c my_thread.c my_thread.h list.c list.h tcb.h)

target_link_libraries(multidisplay ncurses)
cmake_minimum_required(VERSION 3.13)
project(multidisplay C)

set(CMAKE_C_STANDARD 99)

add_executable(server server.c lib/socket_utils.c lib/socket_utils.h lib/error_handling.c lib/error_handling.h port.h lib/point.h lib/area.h)

add_executable(client client.c lib/socket_utils.c lib/socket_utils.h lib/error_handling.c lib/error_handling.h port.h lib/point.h lib/area.h)

target_link_libraries(client ncurses)
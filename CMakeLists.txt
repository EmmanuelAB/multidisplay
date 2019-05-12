cmake_minimum_required(VERSION 3.13)
project(multidisplay C)

set(CMAKE_C_STANDARD 99)

add_executable(server
        animation/server_main.c
        sockets/socket_utils.c
        utils/error_handling.c
        my_thread/my_thread.c
        linked_list/list.c
        utils/error_handling.c
        animation/server.c
        animation/server.h
        parse_json/parse_json.c
        )


add_executable(client
        animation/client_main.c
        animation/client.c
        sockets/socket_utils.c
        utils/error_handling.c
        my_thread/my_thread.c
        linked_list/list.c
        utils/error_handling.c
        )

target_link_libraries(server ncurses json-c m)
target_link_libraries(client ncurses json-c)
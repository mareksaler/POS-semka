cmake_minimum_required(VERSION 3.7)
project(Server C)

set(CMAKE_C_STANDARD 99)

add_executable(Server mainC.c)

target_link_libraries(Server pthread)

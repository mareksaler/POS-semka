cmake_minimum_required(VERSION 3.7)
project(Client C)

set(CMAKE_C_STANDARD 99)

add_executable(Client mainC.c)

target_link_libraries(Client pthread)

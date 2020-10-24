cmake_minimum_required(VERSION 3.17)
project(deflate C)

set(CMAKE_C_STANDARD 99)

add_executable(deflate main.c lib/file_manager/filemanager.c)
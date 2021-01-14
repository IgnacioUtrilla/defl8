cmake_minimum_required(VERSION 3.17)
project(deflate C)

set(CMAKE_C_STANDARD 99)

# Profiler configurations
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(deflate main.c lib/file_manager/filemanager.c)
add_executable(
        staticFrequency
        lib/utils/utils.c
        lib/utils/hash_map/hashMap.c
        lib/utils/map/map.c
        lib/huffman/huffman.c
        lib/huffman/static_frequency/staticFrequency.c
)
target_link_libraries(staticFrequency m)
cmake_minimum_required(VERSION 3.20)
project(cbars VERSION 1.0.0)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -pedantic)

file(GLOB EXAMPLES examples/*.c)

foreach (EXAMPLE ${EXAMPLES})
    get_filename_component(EXEC ${EXAMPLE} NAME_WE)
    add_executable("${EXEC}.out" ${EXAMPLE})
    target_link_libraries("${EXEC}.out" m)
endforeach ()

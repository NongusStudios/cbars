cmake_minimum_required(VERSION 3.20)
project(cbars VERSION 1.0.0)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -pedantic)

include_directories(include)

file(GLOB CBAR_SRC
    src/*.c
)
add_library(cbars
    ${CBAR_SRC}
)

add_executable(a.out examples/colors_and_effects.c)
target_link_libraries(a.out
    cbars
    m
)

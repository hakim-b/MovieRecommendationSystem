cmake_minimum_required(VERSION 3.27)
project(MovieReccomendationSystem C)

set(CMAKE_C_STANDARD 11)

add_executable(MovieReccomendationSystem main.c
        header_files/struct_types.h
)

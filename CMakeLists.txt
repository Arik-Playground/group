cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(nassgroup
    VERSION 0.1
    LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include/)

add_subdirectory(tests)
add_subdirectory(benchmarking)
add_subdirectory(compile_benchmarking)
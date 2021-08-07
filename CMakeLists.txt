cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(nass_group
    VERSION 0.1
    LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include/)
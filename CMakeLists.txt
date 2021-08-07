cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(nass_group
    VERSION 0.1
    LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include/)

add_executable(${PROJECT_NAME}_tests source/nassgroup_tests/tests.cpp)
find_package(Catch2 2 REQUIRED)
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME} Catch2)
target_compile_definitions(${PROJECT_NAME}_tests PRIVATE CATCH_CONFIG_MAIN)

set_target_properties(${PROJECT_NAME}_tests
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
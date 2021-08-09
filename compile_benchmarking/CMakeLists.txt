cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(nassgroup_ct_benchmark
    VERSION 0.1
    LANGUAGES CXX)

add_library(${PROJECT_NAME}_group_construction STATIC EXCLUDE_FROM_ALL construction_only/group.cpp)
target_link_libraries(${PROJECT_NAME}_group_construction PRIVATE nassgroup)
set_target_properties(${PROJECT_NAME}_group_construction
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

add_library(${PROJECT_NAME}_tuple_construction STATIC EXCLUDE_FROM_ALL construction_only/tuple.cpp)
set_target_properties(${PROJECT_NAME}_tuple_construction
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

add_library(${PROJECT_NAME}_group_construction_and_get STATIC EXCLUDE_FROM_ALL construction_and_get/group.cpp)
target_link_libraries(${PROJECT_NAME}_group_construction_and_get PRIVATE nassgroup)
set_target_properties(${PROJECT_NAME}_group_construction_and_get
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)

add_library(${PROJECT_NAME}_tuple_construction_and_get STATIC EXCLUDE_FROM_ALL construction_and_get/tuple.cpp)
set_target_properties(${PROJECT_NAME}_tuple_construction_and_get
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
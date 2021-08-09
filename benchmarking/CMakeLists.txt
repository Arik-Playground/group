cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(nassgroup_benchmark
    VERSION 0.1
    LANGUAGES CXX)

find_package(benchmark 1.3 REQUIRED)

add_executable(${PROJECT_NAME} EXCLUDE_FROM_ALL 
    main.cpp
    creation.cpp
    get.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE nassgroup PRIVATE benchmark::benchmark)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
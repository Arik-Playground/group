cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)

project(nassgroup_tests
    VERSION 0.1
    LANGUAGES CXX)

find_package(Catch2 2 REQUIRED)
add_executable(${PROJECT_NAME} EXCLUDE_FROM_ALL tests.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE nassgroup PRIVATE Catch2::Catch2)
target_compile_definitions(${PROJECT_NAME} PRIVATE CATCH_CONFIG_MAIN)
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO)
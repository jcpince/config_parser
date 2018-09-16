cmake_minimum_required (VERSION 2.8.11)
project(config_parser "C")

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_library(config_parser SHARED "")
add_library(config_parser_static STATIC "")
set_target_properties(config_parser_static PROPERTIES OUTPUT_NAME config_parser)

target_sources(config_parser
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/config_parser.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include/config_parser.h
)
target_sources(config_parser_static
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/config_parser.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include/config_parser.h
)

add_executable(unit_test ${CMAKE_CURRENT_LIST_DIR}/test/test.c)
target_link_libraries(unit_test config_parser)

add_executable(unit_test_static ${CMAKE_CURRENT_LIST_DIR}/test/test.c)
target_link_libraries(unit_test_static config_parser_static)

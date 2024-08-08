cmake_minimum_required(VERSION 3.10)

project(YCorTests)

set(CMAKE_C_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/../include)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/../src)

set(ASM_FILES ${SRC_DIR}/*.S)

set(TEST_SOURCES
    ycor_test1.c
    ycor_test2.c
    ycor_test3.c
    ycor_test4.c
    ycor_test5.c
    ycor_test6.c
    ycor_test7.c
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC} ${SRC_DIR}/*.c ${ASM_FILES})
endforeach()

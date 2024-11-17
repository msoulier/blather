# 1: Set the minimum required version of CMake to be 3.10
cmake_minimum_required(VERSION 3.10)

project(blather
    VERSION 0.0.1
    DESCRIPTION "Mike's personal chat client"
    HOMEPAGE_URL https://github.com/msoulier/blather
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/config.hpp @ONLY)

add_executable(bclient
    bclient.cpp logger.cpp protocol.cpp session.cpp)
add_executable(bserver
    bserver.cpp logger.cpp protocol.cpp session.cpp)

target_include_directories(bclient
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../mikelibc
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../mikelibcpp)

target_include_directories(bserver
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../mikelibc
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../mikelibcpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../mikelibcpp mikebuild)

target_link_libraries(bclient PRIVATE mikecpp)
target_link_libraries(bserver PRIVATE mikecpp)

set(CMAKE_C_COMPILER gcc)

set(CMAKE_DEBUG_POSTFIX d)

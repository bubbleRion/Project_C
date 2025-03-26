cmake_minimum_required(VERSION 3.10)
project(project)

find_library(MYSQL_LIBRARY NAMES mysqlclient)
find_path(MYSQL_INCLUDE_DIR NAMES mysql.h
          PATH_SUFFIXES mysql)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SOURCES "${SOURCE_DIR}/*.c")

add_executable(project ${SOURCES})

target_include_directories(project PRIVATE ${MYSQL_INCLUDE_DIR} ${INCLUDE_DIR})

target_link_libraries(project PRIVATE ${MYSQL_LIBRARY})
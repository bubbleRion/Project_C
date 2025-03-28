cmake_minimum_required(VERSION 3.10)
project(washer)

find_library(MYSQL_LIBRARY NAMES mysqlclient)
find_path(MYSQL_INCLUDE_DIR NAMES mysql.h
          PATH_SUFFIXES mysql)

add_executable(washer washer.c)

target_include_directories(washer PRIVATE ${MYSQL_INCLUDE_DIR})
target_link_libraries(washer PRIVATE ${MYSQL_LIBRARY})
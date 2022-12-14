cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

set(PROJECT_NAME DateTime)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} src/Date.cpp src/Time.cpp src/DateTime.cpp src/DateTime.h)
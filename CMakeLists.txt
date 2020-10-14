cmake_minimum_required(VERSION 3.10)

# Project name
project(p1 VERSION 1.0)

add_executable(p1 main.c)
target_link_libraries(
  p1
  PRIVATE json-c)


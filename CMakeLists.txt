cmake_minimum_required(VERSION 3.10)

# Project name
project(p1 VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

add_subdirectory(tests)

add_library(project_warnings INTERFACE)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

add_executable(p1 main.c json.c math.c www.c util.c command.c)
target_link_libraries(
  p1
  PRIVATE project_warnings
          json-c
	  curl
          m)

set_property(TARGET p1 PROPERTY C_STANDARD 90)



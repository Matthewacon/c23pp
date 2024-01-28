cmake_minimum_required(VERSION 3.25)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(
 c23pp
  VERSION 0.0.0
  LANGUAGES C
  DESCRIPTION "A prototype C23 preprocessor"
)

set(CMAKE_C_STANDARD 99)

#find sources
file(
 GLOB_RECURSE
 TEST_HEADERS
 LIST_DIRECTORIES FALSE
 "include/*.h"
)
file(
 GLOB_RECURSE
 TEST_SOURCES
 LIST_DIRECTORIES FALSE
 "src/*.c"
)

add_executable(c23pp ${TEST_SOURCES})
target_include_directories(
 c23pp
  PUBLIC include
)
target_compile_options(
 c23pp
  PUBLIC
   -Werror
   -Wall
   -Wextra
   -pedantic
   -O0
   -g
   -fno-inline
   -fno-inline-functions
   -fstandalone-debug
   -fsanitize=address
   -fsanitize=undefined
)
target_link_options(
 c23pp
  PUBLIC
   -lasan
   -lubsan
)


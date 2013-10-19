# Copyright (C) 2013, Aldrin D'Souza.
# License: http://opensource.org/licenses/BSD-2-Clause

cmake_minimum_required(VERSION 2.8)
project(crest)
include(ajdMacros)
enable_cpp11()
enable_boost_libraries(system)

file(GLOB SOURCES "impl/*.cpp")
list(APPEND SOURCES "impl/http_parser.c")

file(GLOB HEADERS "impl/*.hpp")
file(GLOB API "api/crest/*.hpp")
list(APPEND HEADERS "impl/http_parser.h")
list(APPEND HEADERS ${API})

include_directories("api")
add_library(crest ${SOURCES} ${HEADERS})

set(LIBRARIES ${Boost_LIBRARIES})
list(APPEND LIBRARIES crest)
build_executables("samples/*.cpp" ${LIBRARIES})
build_tests("tests/*.cpp" ${LIBRARIES})

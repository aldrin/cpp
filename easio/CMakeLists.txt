# Copyright (C) 2013, Aldrin D'Souza.
# License: http://opensource.org/licenses/BSD-2-Clause

cmake_minimum_required(VERSION 2.8)
project(easio)
include(ajdMacros)
enable_cpp11()
enable_boost_libraries(system)

build_executables(main.cpp ${Boost_LIBRARIES})

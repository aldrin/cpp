# Copyright (c) 2013, Aldrin's Notebook.
# http://opensource.org/licenses/BSD-2-Clause
cmake_minimum_required(VERSION 2.8)
project(coro)
include(ajdMacros)
enable_boost_libraries(context coroutine regex filesystem system)
enable_cpp11()
build_tests("*.cpp" ${Boost_LIBRARIES})

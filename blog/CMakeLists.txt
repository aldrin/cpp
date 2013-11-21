# Copyright (c) 2013, Aldrin's Notebook.
# http://opensource.org/licenses/BSD-2-Clause
cmake_minimum_required(VERSION 2.8)
project(blog)
include(ajdMacros)
enable_cpp11()
build_tests("*.cpp" "")

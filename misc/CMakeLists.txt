# Copyright (c) 2013, Aldrin's Notebook.
# http://opensource.org/licenses/BSD-2-Clause

cmake_minimum_required(VERSION 2.8)
project(misc)
include(ajdMacros)
enable_cpp11()

add_executable(tree tree.cpp tree.hpp)
add_test(tree_test tree)

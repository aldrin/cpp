# Copyright (C) 2013, Aldrin D'Souza.
# License: http://opensource.org/licenses/BSD-2-Clause

# enable c++11 features
macro(enable_cpp11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --stdlib=libc++")
    if (${CMAKE_GENERATOR} MATCHES "Xcode")
      set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
      set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    endif()
  endif()
endmacro()

# enable boost header only libraries
macro(enable_boost)
  find_package(Boost REQUIRED)
  include_directories(${Boost_INCLUDE_DIRS})
endmacro()

# enable boost libraries that need to be linked with
macro(enable_boost_libraries)
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_MULTITHREADED ON)
  find_package(Boost REQUIRED COMPONENTS ${ARGV})
  include_directories(${Boost_INCLUDE_DIRS})
  if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    list(APPEND Boost_LIBRARIES "-lrt")
  endif()
endmacro()

# enable warnings. i like them.
macro(enable_warnings)
  if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall --pedantic")
  endif()
endmacro()

# enable includes across projects
macro(enable_crossproject)
  include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")
endmacro()

# build all cpp files in a directory
macro(build_glob)
  file(GLOB sources ${ARGV0})
  set(rest ${ARGV})
  list(GET rest -1 is_test)
  list(REMOVE_AT rest 0 -1)
  foreach(exefile ${sources})
    get_filename_component(exe ${exefile} NAME_WE)
    add_executable(${exe} ${exefile})
    target_link_libraries(${exe} ${rest})
    if(${is_test})
      add_test(${exe} ${exe})
    endif()
  endforeach()
endmacro()

# build as executables.
macro(build_executables)
  set(arguments ${ARGV})
  list(APPEND arguments 0)
  build_glob(${arguments})
endmacro()

# build as tests
macro(build_tests)
  set(arguments ${ARGV})
  list(APPEND arguments 1)
  build_glob(${arguments})
endmacro()


# Settings enabled by default
enable_testing()
enable_warnings()
enable_crossproject()

# Copyright (c) 2013, Aldrin's Notebook.
# http://opensource.org/licenses/BSD-2-Clause

cmake_minimum_required(VERSION 2.8)
project(crypto)

include(ajdMacros)
enable_boost()

find_package(OpenSSL 1.0.1)
if(OPENSSL_FOUND)
  include_directories(${OPENSSL_INCLUDE_DIR})
endif()

add_executable(crypto crypto.cpp crypto.h)
target_link_libraries(crypto ${OPENSSL_CRYPTO_LIBRARY})
add_test(crypto_test crypto)

add_executable(vectors vectors.cpp crypto.h)
target_link_libraries(vectors ${OPENSSL_CRYPTO_LIBRARY})    
add_test(vectors_test vectors)

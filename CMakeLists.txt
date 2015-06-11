cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(LZCompression)



set(LZC_HEADERS
  src/buffer.h
  src/dict.h
  src/lz.h
  src/lz78.h
  src/lzw.h
  src/pool_dict_tree.h
  src/prefix.h
  src/smru_dict.h
)

set(LZC_SOURCES
  src/buffer.cpp
  src/pool_dict_tree.cpp
  src/smru_dict.cpp
)

add_library(lzc ${LZC_HEADERS} ${LZC_SOURCES})
set_property(TARGET lzc PROPERTY CXX_STANDARD 11)
set_property(TARGET lzc PROPERTY CXX_STANDARD_REQUIRED ON)



set(LZC_TEST_HEADERS
  test/prefix.h
)

set(LZC_TEST_SOURCES
  test/buffer.cpp
  test/encoding_decoding.cpp
  test/lz78.cpp
  test/lzw.cpp
  test/main.cpp
  test/pool_dict_tree.cpp
  test/smru_dict.cpp
  test/word_tree_node.cpp
)

add_executable(lzc_test ${LZC_TEST_HEADERS} ${LZC_TEST_SOURCES})
set_property(TARGET lzc_test PROPERTY CXX_STANDARD 11)
set_property(TARGET lzc_test PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(lzc_test pthread gtest lzc)

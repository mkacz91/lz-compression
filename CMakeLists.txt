cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(LZCompression)



set(LZC_HEADERS
  src/bit_buffer.hpp
  src/dictionary_node.hpp
  src/mru_pool.hpp
  src/prefix.hpp
  src/trie_dictionary.hpp
)

set(LZC_SOURCES
  src/bit_buffer.cpp
  src/mru_pool.cpp
  src/trie_dictionary.cpp
)

add_library(lzc ${LZC_HEADERS} ${LZC_SOURCES})
set_property(TARGET lzc PROPERTY CXX_STANDARD 11)
set_property(TARGET lzc PROPERTY CXX_STANDARD_REQUIRED ON)



set(LZC_TEST_HEADERS
  test/prefix.hpp
)

set(LZC_TEST_SOURCES
  test/bit_buffer.cpp
  test/dictionary_node.cpp
  test/main.cpp
  test/mru_pool.cpp
  test/trie_dictionary.cpp
)

add_executable(lzc_test ${LZC_TEST_HEADERS} ${LZC_TEST_SOURCES})
set_property(TARGET lzc_test PROPERTY CXX_STANDARD 11)
set_property(TARGET lzc_test PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(lzc_test pthread gtest lzc)

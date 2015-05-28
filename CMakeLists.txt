cmake_minimum_required(VERSION 2.8)

project(LZCompression)



set(LZC_HEADERS
  src/bit_buffer.hpp
  src/dictionary_node.hpp
  src/prefix.hpp
)

set(LZC_SOURCES
  src/bit_buffer.cpp
)

add_library(lzc ${LZC_HEADERS} ${LZC_SOURCES})



set(LZC_TEST_HEADERS
  test/prefix.hpp
)

set(LZC_TEST_SOURCES
  test/bit_buffer.cpp
  test/dictionary_node.cpp
  test/main.cpp
)

add_executable(lzc_test ${LZC_TEST_HEADERS} ${LZC_TEST_SOURCES})
target_link_libraries(lzc_test pthread gtest lzc)

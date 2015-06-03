cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(LZCompression)



set(LZC_HEADERS
  src/buffer.hpp
  src/lz78.hpp
  src/mru_dict.hpp
  src/mru_encode_dict.hpp
  src/prefix.hpp
)

set(LZC_SOURCES
  src/buffer.cpp
  src/mru_dict.cpp
  src/mru_encode_dict.cpp
)

add_library(lzc ${LZC_HEADERS} ${LZC_SOURCES})
set_property(TARGET lzc PROPERTY CXX_STANDARD 11)
set_property(TARGET lzc PROPERTY CXX_STANDARD_REQUIRED ON)



set(LZC_TEST_HEADERS
  test/prefix.hpp
)

set(LZC_TEST_SOURCES
  test/buffer.cpp
  test/encode_dict_node.cpp
  test/main.cpp
  test/mru_dict.cpp
)

add_executable(lzc_test ${LZC_TEST_HEADERS} ${LZC_TEST_SOURCES})
set_property(TARGET lzc_test PROPERTY CXX_STANDARD 11)
set_property(TARGET lzc_test PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(lzc_test pthread gtest lzc)

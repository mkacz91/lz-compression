cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(LZCompression)



set(LZC_HEADERS
  src/buffer.h
  src/dict.h
  src/huffman.h
  src/lz.h
  src/lz78.h
  src/lzw.h
  src/mra_dict.h
  src/pool_dict_tree.h
  src/pool_dict.h
  src/prefix.h
  src/smru_dict.h
  src/wmru_dict.h
)

set(LZC_SOURCES
  src/buffer.cpp
  src/huffman.cpp
  src/pool_dict_tree.cpp
  src/smru_dict.cpp
  src/wmru_dict.cpp
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
  test/huffman.cpp
  test/lz78.cpp
  test/lzw.cpp
  test/main.cpp
  test/mra_dict.cpp
  test/pool_dict_tree.cpp
  test/smru_dict.cpp
  test/word_tree_node.cpp
)

add_executable(lzc_test ${LZC_TEST_HEADERS} ${LZC_TEST_SOURCES})
set_property(TARGET lzc_test PROPERTY CXX_STANDARD 11)
set_property(TARGET lzc_test PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(lzc_test pthread gtest lzc)



add_executable(encode src/encode.cpp)
set_property(TARGET encode PROPERTY CXX_STANDARD 11)
set_property(TARGET encode PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(encode lzc)



set(LZC_BENCHMARK_HEADERS
  benchmark/benchmark.h
  benchmark/input_provider.h
  benchmark/prefix.h
)

set(LZC_BENCHMARK_SOURCES
  benchmark/benchmark.cpp
  benchmark/dict_size.cpp
  benchmark/input_provider.cpp
  benchmark/main.cpp
  benchmark/time.cpp
  benchmark/incremental.cpp
)

add_executable(lzc_benchmark ${LZC_BENCHMARK_HEADERS} ${LZC_BENCHMARK_SOURCES})
set_property(TARGET lzc_benchmark PROPERTY CXX_STANDARD 11)
set_property(TARGET lzc_benchmark PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(lzc_benchmark lzc)

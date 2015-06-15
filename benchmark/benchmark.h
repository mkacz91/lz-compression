#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "prefix.h"
#include <map>
#include <vector>

#include "../src/lz.h"

#include "input_provider.h"

struct Sample {
    int input_bits;
    int codewords;
    int lz_bits;
    int lz_encoding_ms;
    int lz_decoding_ms;
    int huffman_bits;
    int huffman_encoding_ms;
    int huffman_decoding_ms;
};

class Benchmark {
public:

    typedef std::map<string, std::vector<Sample>> Result;

    void register_encoder (string const& name, Lz const* encoder);
 
    static Sample run (Buffer const& input, int repeat_cnt, Lz const& encoder);

//    Result run (InputProvider& provider, int repeat_cnt = 10) const;

private:
    std::map<string, Lz const*> m_encoders;
};

#endif // BENCHMARK_H

#include "benchmark.h"

#include "../src/huffman.h"

void Benchmark::register_encoder (string const& name, Lz const* encoder) {
    assert(!name.empty());
    assert(m_encoders.count(name) == 0);
    m_encoders[name] = encoder;
}
/*
Benchmark::Result
Benchmark::run (InputProvider& provider, int repeat_cnt) const {
    Result result;
    int n = provider.max_get_cnt();
    while (n --> 0) {
        Buffer const& input = provider.get();
        cout << "Running samples for input size: " << input.size() << endl;
        for (auto ne : m_encoders) {
            string name = ne.first;
            Lz const* encoder = ne.second;
            cout << "    " << name << "... ";
            result[name].push_back(run(input, repeat_cnt, encoder));
            cout << "done" << endl;
        }
    }
    return result;
}
*/
int avg_ms (std::vector<milliseconds> const& times) {
    uint64_t sum = 0;
    for (size_t i = 0; i < times.size(); ++i)
        sum += times[i].count();
    return sum / times.size();
}

Sample Benchmark::run (Buffer const& input, int repeat_cnt, Lz const& encoder) {
    Sample sample;
    sample.input_bits = input.size();
    std::vector<milliseconds> lz_encoding_ms;
    std::vector<milliseconds> lz_decoding_ms;
    std::vector<milliseconds> huffman_encoding_ms;
    std::vector<milliseconds> huffman_decoding_ms;
    while (repeat_cnt --> 0) {
        auto t0 = system_clock::now();
        Buffer lz_output = encoder.encode(input);
        sample.codewords = lz_output.size() / encoder.codeword_bits();
        sample.lz_bits = lz_output.size();
        auto t1 = system_clock::now();
        Buffer huffman_output = Huffman::encode(lz_output);
        sample.huffman_bits = huffman_output.size();
        auto t2 = system_clock::now();
        Buffer huffman_input = Huffman::decode(huffman_output);
        auto t3 = system_clock::now();
        Buffer lz_input = encoder.decode(huffman_input);
        auto t4 = system_clock::now();
        if (!(lz_input == input)) {
            throw "Something went wrong!";
        }
        lz_encoding_ms.push_back(duration_cast<milliseconds>(t1 - t0));
        lz_decoding_ms.push_back(duration_cast<milliseconds>(t4 - t3));
        huffman_encoding_ms.push_back(duration_cast<milliseconds>(t2 - t1));
        huffman_decoding_ms.push_back(duration_cast<milliseconds>(t3 - t2));
    }
    sample.lz_encoding_ms = avg_ms(lz_encoding_ms);
    sample.lz_decoding_ms = avg_ms(lz_decoding_ms);
    sample.huffman_encoding_ms = avg_ms(huffman_decoding_ms);
    sample.huffman_decoding_ms = avg_ms(huffman_decoding_ms);
    return sample;
}

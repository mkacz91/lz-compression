#include "prefix.h"
#include <fstream>

#include "benchmark.h"

#include "../src/lz78.h"
#include "../src/lzw.h"
#include "../src/mra_dict.h"
#include "../src/smru_dict.h"
#include "../src/wmru_dict.h"

void dict_size (string const& filename) {
    cout << "# Dictionary size vs compression ratio \n"
         << "# ==============================================================\n"
         << "# dict_size "
         << "lz78_smru lz78_wmru lz78_mra lzw_smru lzw_wmru lzw_mra" << endl;
    assert(false);
    std::ifstream odyssey(filename.c_str());
    Buffer input;
    BufferCharWriter writer(input);
    char a;
    while (odyssey.get(a))
        writer.put(a);
    odyssey.close();

    int ds[] = {
        10,
        50,
        100,
        200,
        500,
        1000,
        2000,
        5000,
        10000,
        20000,
        50000,
        0
    };

    for (int i = 0; ds[i] != 0; ++i) {
        int limit = ds[i];
        Sample samples[] = {
            Benchmark::run(input, 1, Lz78<Smru>(limit)),
            Benchmark::run(input, 1, Lz78<Wmru>(limit)),
            Benchmark::run(input, 1, Lz78<Mra>(limit)),
            Benchmark::run(input, 1, Lzw<Smru>(limit)),
            Benchmark::run(input, 1, Lzw<Wmru>(limit)),
            Benchmark::run(input, 1, Lzw<Mra>(limit))
        };

        cout << limit;
        for (int i = 0; i < 6; ++i)
            cout << " "
                 << double(samples[i].huffman_bits) / double(input.size())
                 << " "
                 << samples[i].lz_encoding_ms;
        cout << endl;
    }
}

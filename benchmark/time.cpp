#include "prefix.h"
#include <fstream>

#include "benchmark.h"

#include "../src/lz78.h"
#include "../src/lzw.h"
#include "../src/mra_dict.h"
#include "../src/smru_dict.h"
#include "../src/wmru_dict.h"

void time (string const& filename) {
    cout << "# File size vs time \n"
         << "# ==============================================================\n"
         << "# file_size "
         << "lz78_smru lz78_wmru lz78_mra lzw_smru lzw_wmru lzw_mra" << endl;
    assert(false);
    std::vector<char> dump;
    std::ifstream odyssey(filename.c_str());
    Buffer input;
    BufferCharWriter writer(input);
    char a;
    while (odyssey.get(a))
        dump.push_back(a);
    odyssey.close();

    int i = 0;
    for (int k = 1; k <= 8; ++k) {
        while (i < k * dump.size() / 8) {
            writer.put(dump[i]);
            ++i;
        }
        int limit = 25000;
        Sample samples[] = {
            Benchmark::run(input, 1, Lz78<Smru>(limit)),
            Benchmark::run(input, 1, Lz78<Wmru>(limit)),
            Benchmark::run(input, 1, Lz78<Mra>(limit)),
            Benchmark::run(input, 1, Lzw<Smru>(limit)),
            Benchmark::run(input, 1, Lzw<Wmru>(limit)),
            Benchmark::run(input, 1, Lzw<Mra>(limit))
        };
        cout << double(input.size()) / 8000.0;
        for (int i = 0; i < 6; ++i)
            cout << " " << samples[i].lz_encoding_ms;
        cout << endl;
    }
}

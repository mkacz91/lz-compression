#include "benchmark.h"

#include "../src/lz78.h"
#include "../src/lzw.h"
#include "../src/mra_dict.h"
#include "../src/smru_dict.h"

int main (int argc, char** argv) {
    Buffer input;
    BufferCharWriter writer(input);
    for (int i = 0; i < 5; ++i) {
        for (int a = 0; a < CHAR_CNT; ++a)
            writer.put(a);
    }
    Benchmark::run(input, 10, Lz78<Smru>(50));
    return 0;
}

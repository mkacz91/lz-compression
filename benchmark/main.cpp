#include "prefix.h"

extern void dict_size (string const& filename);
extern void time (string const& filename);
extern void incremental (string const& filename);

int main (int argc, char** argv) {
    //dict_size("../benchmark/data/duck.bmp");
    //time("../benchmark/data/odyssey.txt");
    incremental("../benchmark/data/aaa.txt");
    return 0;
}

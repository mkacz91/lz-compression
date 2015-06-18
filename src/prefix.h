#ifndef PREFIX_H
#define PREFIX_H

#include <cassert>

#include <iostream>
using std::cin;
using std::cout;
using std::endl;
using std::flush;

#include <string>
using std::string;

#include <cstdint>
// TODO: Investigate 32 vs 64 bit
typedef uint32_t word;
int const WORD_BITS = 32;
int const CHAR_BITS = 8;
int const WORD_CHARS = WORD_BITS / CHAR_BITS;
int const INT_BITS = 32; // Well...
word const NULL_WORD = 0;
word const ONES_MASK = ~NULL_WORD;
int const CHAR_CNT = 256;

#include <algorithm>
using std::min;
using std::max;
using std::copy;
using std::equal;

#include <utility>
using std::swap;
using std::pair;
using std::make_pair;

#include <chrono>
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::duration_cast;

#define UNUSED(var) (void)(var)

inline word lshift (word x, int n) {
    if (n >= WORD_BITS)
        return NULL_WORD;
    if (n < 0) {
        if (n <= -WORD_BITS)
            return NULL_WORD;
        return x >> (-n);
    }
    return x << n;
}

inline word rshift (word x, int n) {
    if (n >= WORD_BITS)
        return NULL_WORD;
    if (n < 0) {
        if (n <= -WORD_BITS)
            return NULL_WORD;
        return x << (-n);
    }
    return x >> n;
}

inline int ceil_log2 (int n) {
    int i = 0;
    int p = 1;
    while (p < n) {
        ++i;
        p *= 2;
    }
    return i;
}

template <typename T>
inline T ceil_div (T a, T b) {
    T c = a / b;
    return a == c * b ? c : c + 1;
}

inline word char_to_word (char a) {
    // This is some evil hacking. I regret everything!
    return *reinterpret_cast<unsigned char*>(&a);
}

#endif // PREFIX_H

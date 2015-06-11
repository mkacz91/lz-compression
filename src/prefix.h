#ifndef PREFIX_H
#define PREFIX_H

#include <cassert>

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <cstdint>
// TODO: Investigate 32 vs 64 bit
typedef uint32_t word;
int const WORD_LENGTH = 32;
int const CHAR_LENGTH = 8;
word const NULL_WORD = 0;
word const ONES_MASK = ~NULL_WORD;
int const CHAR_CNT = 256;

#include <algorithm>
using std::min;
using std::max;
using std::copy;

#include <utility>
using std::swap;
using std::pair;
using std::make_pair;

#define UNUSED(var) (void)(var)

inline word lshift (word x, int n) {
    if (n >= WORD_LENGTH)
        return NULL_WORD;
    if (n < 0) {
        if (n <= -WORD_LENGTH)
            return NULL_WORD;
        return x >> (-n);
    }
    return x << n;
}

inline word rshift (word x, int n) {
    if (n >= WORD_LENGTH)
        return NULL_WORD;
    if (n < 0) {
        if (n <= -WORD_LENGTH)
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

#endif // PREFIX_H

#ifndef PREFIX_HPP
#define PREFIX_HPP

#include <cassert>

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <limits>
typedef unsigned int uint;
typedef uint word;
int const WORD_LENGTH = std::numeric_limits<word>::digits;
int const CHAR_LENGTH = 8;
word const NULL_WORD = 0u;
word const ONES_MASK = ~NULL_WORD;

#include <algorithm>
using std::min;
using std::max;
using std::copy;

#include <utility>
using std::swap;
using std::pair;
using std::make_pair;

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
    return p;
}

#endif // PREFIX_HPP

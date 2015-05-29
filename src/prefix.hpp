#ifndef PREFIX_HPP
#define PREFIX_HPP

#include <cassert>

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

#endif // PREFIX_HPP

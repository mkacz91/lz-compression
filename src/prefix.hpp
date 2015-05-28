#ifndef PREFIX_HPP
#define PREFIX_HPP

#include <cassert>

#include <string>
using std::string;

#include <limits>
typedef unsigned int uint;
typedef uint word;
int const WORD_LENGTH = std::numeric_limits<word>::digits;
word const NULL_WORD = 0u;
word const ONES_MASK = ~NULL_WORD;

#include <algorithm>
using std::min;
using std::max;

#endif // PREFIX_HPP

#ifndef DICT_BASE_H
#define DICT_BASE_H

#include "prefix.h"

// **Note:** The classes defined here do not serve the purpose of making it
// possible to use dictionaries in a polymorphic way (they are intended to be
// embedded in the LZ78/LZW factorization algorithms by means templatization).
// They exist rather to impose interface and provide a place for common
// documentation.

// DictBase
// =============================================================================
//
// Base class for all ditionaries to be used by LZ78/LZW encoders/decoders.
class DictBase {
public:
    // Creates an empty dictionary with given limit, which is the upper bound
    // for the number of codewords. Unlimited dictionaries should set `limit`
    // to `0`.
    explicit DictBase (int limit);

    // Returns the upper bound for the number of codewords. Always 0 for
    // unlimited dictionaries.
    int limit () const;

    // Marks the codeword with number `i` as permanent. Limited dictionaries
    // are forbidden to remove such codewords.
    virtual void make_permanent (int i) = 0;

protected:
    // The upper bound for the number of codewords. Irrelevant in unlimited
    // dictionaries and equal to 0.
    int m_limit;
};

inline DictBase::DictBase (int limit) :
    m_limit(limit)
{
    assert(limit >= 0);
}

inline int DictBase::limit () const {
    return m_limit;
}

// EncodeDictBase
// =============================================================================
//
// Base class for all dictionaries specialized for **encoding**. Derived class
// should also inherit from `DictBase`.
//
// The dictionary behaves like a growing automaton. It exposes a method named
// `try_char()` which emits codeword number whenever the maximum match is found.
class EncodeDictBase {
public:
    // Advances the internal state of the automaton, trying to match the
    // currently matched codeword extended by another letter, i.e., `a`.
    //
    // If there is a match, the result is -1, meaning that a longer match may
    // exist. Non-negative result indicates success. It means that the word
    // composed of characters provided after the last successful call to
    // `try_char()`, excluding the current one, is the longest matching
    // codeword present in the dictionary. The result is then the number of the
    // matching codeword, or 0 for the empty codeword.
    //
    // On success, a new codeword is added. In limited dictionaries this may
    // involve discarding some other codeword.
    virtual int try_char (char a) = 0;

    // Returns the number of the currently matched codeword, that may still be
    // extended.
    virtual int peek_codeword_no () const = 0;
};

// Codeword
// =============================================================================
//
// Representation of a codeword. A codeword is a subsequence of the input.
// Such subsequence is determined by its starting index `begin` and its
// `length`.
struct Codeword {
    int begin;
    int length;

    // Creates a new codeword with given starting index and length.
    Codeword (int begin, int length);

    // Returns `true` if `cw` is equal to this codeword.
    inline bool operator == (Codeword const& cw) const;
};

inline Codeword::Codeword (int begin, int length) :
    begin(begin),
    length(length)
{
    /* Do nothing */
}

inline bool Codeword::operator == (Codeword const& cw) const {
    return begin == cw.begin && length == cw.length;
}

// DecodeDictBase
// =============================================================================
//
// Base class for all dictionaries specialized for **decoding**. Derived class
// should also inherit from `DictBase`.
class DecodeDictBase {
    // Adds new codeword to the dictionary. The new codeword starts at `begin`
    // and is a one-letter extension of the existing codeword `i`.
    virtual void add_extension (int i, int begin) = 0;

    // Returns the `i`th codeword.
    virtual Codeword const& codeword (int i) const = 0;
};

#endif // DICT_BASE_H
